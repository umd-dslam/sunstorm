use clap::{error::ErrorKind, CommandFactory, Parser};
use std::net::SocketAddr;
use std::thread::{self, JoinHandle};
use tokio::sync::mpsc;
use tokio_util::sync::CancellationToken;
use tracing::info;
use tracing::metadata::LevelFilter;
use url::Url;
use xactserver::pg::PgWatcher;
use xactserver::{init_tracing, start_http_server, Manager, Node, NodeId, XsMessage, DUMMY_URL};

#[derive(Parser)]
#[command(author, version, about, long_about = None)]
struct Cli {
    #[arg(
        long,
        value_parser,
        default_value = "127.0.0.1:10000",
        help = "Address to listen for connections from postgres"
    )]
    listen_pg: SocketAddr,

    #[arg(
        long,
        value_parser,
        default_value = "postgresql://cloud_admin@localhost:55433/postgres",
        help = "Address of postgres to connect to"
    )]
    connect_pg: String,

    #[arg(
        long,
        default_value = "128",
        help = "Maximum size of the postgres connection pool"
    )]
    max_conn_pool_size_pg: u32,

    #[arg(
        long,
        value_parser,
        default_value = "http://localhost:23000",
        help = "Comma-separated list of addresses of all xact servers"
    )]
    nodes: String,

    #[arg(
        long,
        value_parser,
        default_value = "0.0.0.0:23000",
        help = "Address to listen for connections from other xact servers"
    )]
    listen_peer: SocketAddr,

    #[arg(
        long,
        default_value = "64",
        help = "Maximum size of the connection pool per peer"
    )]
    max_conn_pool_size_peer: u32,

    #[arg(
        long,
        value_parser,
        default_value = "0",
        help = "Numeric id of the current node. Used as an 0-based index of the --nodes list"
    )]
    node_id: NodeId,

    #[arg(
        long,
        value_parser,
        default_value = "127.0.0.1:8080",
        help = "Address to listen for http requests"
    )]
    listen_http: SocketAddr,

    #[arg(long, help = "Disable ANSI color codes in logs")]
    no_ansi: bool,
}

#[tokio::main]
async fn main() -> anyhow::Result<()> {
    let cli = Cli::parse();

    init_tracing(LevelFilter::INFO, cli.no_ansi)?;

    let connect_pg = Url::parse(&cli.connect_pg).unwrap_or_else(|err| {
        invalid_arg_error(&format!(
            "unable to parse postgres url in '--connect-pg': {}",
            err
        ))
    });

    if !connect_pg.scheme().eq("postgresql") {
        invalid_arg_error(&format!(
            "invalid scheme '{}' in '--connect-pg'. Must be 'postgresql'",
            connect_pg.scheme()
        ));
    }

    let node_addrs = parse_node_addresses(cli.nodes);
    let cancel = CancellationToken::new();

    let (pg_watcher_handle, watcher_rx) = start_pg_watcher(cli.listen_pg, cancel.clone());
    let (node_handle, node_rx) = start_peer_listener(cli.listen_peer, cancel.clone());
    let manager_handle = start_manager(
        cli.node_id,
        connect_pg,
        cli.max_conn_pool_size_pg,
        node_addrs,
        cli.max_conn_pool_size_peer,
        watcher_rx,
        node_rx,
        cancel.clone(),
    );
    let http_server_handle = start_http_server(cli.listen_http, cancel.clone());

    tokio::select! {
        _ = tokio::signal::ctrl_c() => {},
        _ = cancel.cancelled() => {},
    }

    cancel.cancel();

    for handle in [
        pg_watcher_handle,
        node_handle,
        manager_handle,
        http_server_handle,
    ] {
        handle.join().unwrap()?;
    }

    Ok(())
}

fn invalid_arg_error(msg: &str) -> ! {
    Cli::command().error(ErrorKind::InvalidValue, msg).exit();
}

fn parse_node_addresses(nodes: String) -> Vec<Url> {
    let node_addresses: Vec<Url> = nodes
        .split(',')
        .map(|addr| {
            if addr.to_lowercase() == "dummy" {
                return DUMMY_URL.clone();
            }

            let node_url = Url::parse(addr).unwrap_or_else(|err| {
                invalid_arg_error(&format!("invalid value '{}' in '--nodes': {}", addr, err))
            });

            let scheme = node_url.scheme();
            if !["http", "https"].contains(&scheme) {
                invalid_arg_error(&format!(
                    "invalid scheme '{}' in '--nodes'. Must be either http or https",
                    scheme
                ));
            }

            node_url
        })
        .collect();
    node_addresses
}

type HandleAndReceiver = (
    JoinHandle<Result<(), anyhow::Error>>,
    mpsc::Receiver<XsMessage>,
);

fn start_pg_watcher(listen_pg: SocketAddr, cancel: CancellationToken) -> HandleAndReceiver {
    let (watcher_tx, watcher_rx) = mpsc::channel(100);

    info!("Listening to PostgreSQL on {}", listen_pg);
    let pg_watcher = PgWatcher::new(listen_pg, watcher_tx);
    let handle = thread::Builder::new()
        .name("pg watcher".into())
        .spawn(move || {
            tokio::runtime::Builder::new_current_thread()
                .enable_all()
                .build()?
                .block_on(pg_watcher.run(cancel))?;
            Ok(())
        })
        .unwrap();

    (handle, watcher_rx)
}

fn start_peer_listener(listen_peer: SocketAddr, cancel: CancellationToken) -> HandleAndReceiver {
    let (node_tx, node_rx) = mpsc::channel(100);

    info!("Listening to peers on {}", listen_peer);
    let node = Node::new(listen_peer, node_tx);
    let handle = thread::Builder::new()
        .name("network node".into())
        .spawn(move || {
            tokio::runtime::Builder::new_current_thread()
                .enable_all()
                .build()?
                .block_on(node.run(cancel))?;
            Ok(())
        })
        .unwrap();

    (handle, node_rx)
}

#[allow(clippy::too_many_arguments)]
fn start_manager(
    node_id: NodeId,
    connect_pg: Url,
    max_conn_pool_size_pg: u32,
    node_addrs: Vec<Url>,
    max_conn_pool_size_peer: u32,
    watcher_rx: mpsc::Receiver<XsMessage>,
    node_rx: mpsc::Receiver<XsMessage>,
    cancel: CancellationToken,
) -> JoinHandle<Result<(), anyhow::Error>> {
    let manager = Manager::new(
        node_id,
        connect_pg,
        max_conn_pool_size_pg,
        node_addrs,
        max_conn_pool_size_peer,
        watcher_rx,
        node_rx,
    );

    thread::Builder::new()
        .name("xact manager".into())
        .spawn(move || {
            tokio::runtime::Builder::new_multi_thread()
                .enable_all()
                .build()?
                .block_on(manager.run(cancel))?;
            Ok(())
        })
        .unwrap()
}
