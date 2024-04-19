use hyper::Server;
use hyper::{header::CONTENT_TYPE, Body, Request, Response};
use prometheus::{Encoder, TextEncoder};
use routerify::{Router, RouterService};
use std::net::{SocketAddr, TcpListener};
use std::thread::{self, JoinHandle};
use tokio_util::sync::CancellationToken;
use tracing::info;

pub fn start_http_server(
    listen_http: SocketAddr,
    cancel: CancellationToken,
) -> JoinHandle<Result<(), anyhow::Error>> {
    let router_builder = Router::builder().get("/metrics", prometheus_metrics_handler);

    thread::Builder::new()
        .name("http".into())
        .spawn(move || {
            let listener = TcpListener::bind(listen_http)?;

            info!("Listening to HTTP on {}", listener.local_addr()?);

            let service =
                RouterService::new(router_builder.build().map_err(|err| anyhow::anyhow!(err))?)
                    .unwrap();
            let runtime = tokio::runtime::Builder::new_current_thread()
                .enable_all()
                .build()?;

            let _guard = runtime.enter();
            let server = Server::from_tcp(listener)?.serve(service);
            runtime.block_on(async {
                tokio::select! {
                    res = server => res,
                    _ = cancel.cancelled() => Ok(()),
                }
            })?;

            info!("HTTP server stopped");

            Ok(())
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
