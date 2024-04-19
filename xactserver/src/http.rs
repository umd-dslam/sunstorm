use crate::{NodeId, XactData, XsMessage};
use anyhow::Context;
use bit_set::BitSet;
use hyper::Server;
use hyper::{header::CONTENT_TYPE, Body, Request, Response};
use prometheus::{Encoder, TextEncoder};
use rand::distributions::{Bernoulli, Distribution};
use routerify::{ext::RequestExt, Router, RouterService};
use std::borrow::Cow;
use std::net::{SocketAddr, TcpListener};
use std::sync::atomic::{AtomicU32, Ordering};
use std::sync::Arc;
use std::thread::{self, JoinHandle};
use std::time::Duration;
use tokio::sync::{mpsc, oneshot};
use tokio::task::JoinSet;
use tokio_util::sync::CancellationToken;
use tracing::info;
use url::form_urlencoded;

struct State {
    local_xact_tx: mpsc::Sender<XsMessage>,
    node_id: NodeId,
    total_participants: usize,
}

pub fn start_http_server(
    listen_http: SocketAddr,
    local_xact_tx: mpsc::Sender<XsMessage>,
    node_id: NodeId,
    total_participants: usize,
    cancel: CancellationToken,
) -> JoinHandle<Result<(), anyhow::Error>> {
    thread::Builder::new()
        .name("http".into())
        .spawn(move || {
            tokio::runtime::Builder::new_current_thread()
                .enable_all()
                .build()?
                .block_on(async {
                    let _drop_guard = cancel.clone().drop_guard();
                    let router_builder = Router::builder()
                        .data(State {
                            local_xact_tx,
                            node_id,
                            total_participants,
                        })
                        .get("/metrics", prometheus_metrics_handler)
                        .get("/simulate", workload_simulator_handler);

                    let listener = TcpListener::bind(listen_http)?;

                    info!("Listening to HTTP on {}", listener.local_addr()?);

                    let service = RouterService::new(
                        router_builder.build().map_err(|err| anyhow::anyhow!(err))?,
                    )
                    .unwrap();

                    let server = Server::from_tcp(listener)?.serve(service);

                    tokio::select! {
                        res = server => res?,
                        _ = cancel.cancelled() => {},
                    };

                    info!("HTTP server stopped");

                    Ok::<(), anyhow::Error>(())
                })
                .context("Failed to start HTTP server")
        })
        .unwrap()
}

async fn prometheus_metrics_handler(_req: Request<Body>) -> anyhow::Result<Response<Body>> {
    let mut buffer = vec![];
    let encoder = TextEncoder::new();

    let metrics = prometheus::gather();
    encoder.encode(&metrics, &mut buffer).unwrap();

    let response = Response::builder()
        .status(200)
        .header(CONTENT_TYPE, encoder.format_type())
        .body(Body::from(buffer))
        .unwrap();

    Ok(response)
}

#[derive(Clone, Debug)]
enum StoppingCondition {
    Duration(Duration),
    NumTransactions(u32),
}

async fn workload_simulator_handler(req: Request<Body>) -> anyhow::Result<Response<Body>> {
    let Some(query) = req.uri().query() else {
        return Ok(Response::builder()
            .status(400)
            .body(Body::from("missing query parameters"))
            .unwrap());
    };

    let state = req.data::<State>().unwrap();
    let params: Vec<(Cow<str>, Cow<str>)> = form_urlencoded::parse(query.as_bytes()).collect();
    let mut duration_sec = None;
    let mut ntxn = None;
    let mut nclient = 1;
    let mut pdist = None;

    for (key, value) in params {
        match key.as_ref() {
            "duration" => duration_sec = Some(value.parse::<u64>()?),
            "ntxn" => ntxn = Some(value.parse::<u32>()?),
            "nclient" => nclient = value.parse::<u32>()?,
            "pdist" => pdist = Some(value),
            _ => {}
        }
    }

    let stopping_condition = if duration_sec.is_some() && ntxn.is_some() {
        return Ok(Response::builder()
            .status(400)
            .body(Body::from(
                "cannot specify both 'duration' and 'ntxn' query parameters",
            ))
            .unwrap());
    } else if let Some(duration) = duration_sec {
        let duration = Duration::from_secs(duration);
        StoppingCondition::Duration(duration)
    } else if let Some(ntxn) = ntxn {
        if ntxn == 0 {
            return Ok(Response::builder()
                .status(400)
                .body(Body::from("'ntxn' must be greater than 0"))
                .unwrap());
        }
        StoppingCondition::NumTransactions(ntxn)
    } else {
        return Ok(Response::builder()
            .status(400)
            .body(Body::from("missing 'duration' or 'ntxn' query parameter"))
            .unwrap());
    };

    let mut pdist = if let Some(distribution) = pdist {
        distribution
            .split(',')
            .map(|s| s.parse::<f64>())
            .collect::<Result<_, _>>()?
    } else {
        vec![1.0; state.total_participants]
    };
    // Always include the local node
    pdist[state.node_id.0 as usize] = 1.0;

    info!(
        "Simulating workload with the follwing parameters:
        {stopping_condition:?},
        number of clients: {nclient},
        participant distribution: {pdist:?}"
    );

    let cancel = CancellationToken::new();
    let sent_transactions = Arc::new(AtomicU32::new(0));
    let mut join_set = JoinSet::new();
    for _ in 0..nclient {
        let local_xact_tx = state.local_xact_tx.clone();
        let cancel = cancel.clone();
        let sent_transactions = sent_transactions.clone();
        let stopping_condition = stopping_condition.clone();
        let pdist = pdist
            .iter()
            .map(|&p| Bernoulli::new(p))
            .collect::<Result<Vec<_>, _>>()?;
        join_set.spawn(async move {
            while !cancel.is_cancelled() {
                let (commit_tx, commit_rx) = oneshot::channel();

                let participants = {
                    let mut participants = BitSet::new();
                    let mut rng = rand::thread_rng();
                    for (i, distribution) in pdist.iter().enumerate() {
                        if distribution.sample(&mut rng) {
                            participants.insert(i);
                        }
                    }
                    participants
                };

                if participants.len() > 1 {
                    let message = XsMessage::LocalXact {
                        data: XactData::Simulated { participants },
                        commit_tx,
                    };

                    local_xact_tx.send(message).await.unwrap();

                    commit_rx.await.unwrap();
                }

                let sent = sent_transactions.fetch_add(1, Ordering::SeqCst);
                if let StoppingCondition::NumTransactions(ntxn) = stopping_condition {
                    if sent >= ntxn - 1 {
                        break;
                    }
                }
            }
        });
    }

    match stopping_condition {
        StoppingCondition::Duration(duration) => {
            tokio::time::sleep(duration).await;
        }
        StoppingCondition::NumTransactions(ntxn) => {
            while sent_transactions.load(Ordering::SeqCst) < ntxn {
                tokio::time::sleep(Duration::from_millis(100)).await;
            }
        }
    }

    cancel.cancel();

    for _ in 0..nclient {
        join_set.join_next().await;
    }

    let result = format!(
        "Sent {} transactions",
        sent_transactions.load(Ordering::SeqCst)
    );
    Ok(Response::builder()
        .status(200)
        .body(Body::from(result))
        .unwrap())
}
