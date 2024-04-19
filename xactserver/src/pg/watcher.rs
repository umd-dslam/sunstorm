//! # PostgreSQL watcher
//!
//! This module contains [`PgWatcher`] which watches for new transaction data from the
//! `remotexact` plugin in postgres.
//!  
use crate::{RollbackInfo, RollbackReason, XactData, XsMessage};
use anyhow::Context;
use bytes::{BufMut, Bytes, BytesMut};
use neon_postgres_backend::{self, AuthType, PostgresBackend, PostgresBackendTCP, QueryError};
use neon_pq_proto::{BeMessage, FeMessage};
use std::net::SocketAddr;
use tokio::{
    io::{AsyncRead, AsyncWrite},
    sync::{mpsc, oneshot},
};
use tokio_util::sync::CancellationToken;
use tracing::{debug, error, info};

/// A `PgWatcher` listens for new connections from a postgres instance. For each
/// new connection, a [`PostgresBackend`] is created in a new thread. This postgres
/// backend will receive the transaction read/write set and forward this data to
/// [`XactServer`].
///
/// [`PostgresBackend`]: neon_postgres_backend::PostgresBackend
/// [`XactServer`]: crate::XactServer
///
pub struct PgWatcher {
    listen_pg: SocketAddr,
    local_xact_tx: mpsc::Sender<XsMessage>,
}

impl PgWatcher {
    pub fn new(listen_pg: SocketAddr, local_xact_tx: mpsc::Sender<XsMessage>) -> Self {
        Self {
            listen_pg,
            local_xact_tx,
        }
    }

    pub async fn run(&self, cancel: CancellationToken) -> anyhow::Result<()> {
        let listener = tokio::net::TcpListener::bind(self.listen_pg)
            .await
            .context("Failed to start postgres watcher")?;

        loop {
            tokio::select! {
                msg = listener.accept() => {
                    match msg {
                        Ok((socket, peer_addr)) => {
                            debug!("accepted connection from {}", peer_addr);
                            tokio::spawn(Self::conn_main(self.local_xact_tx.clone(), socket));
                        }
                        Err(err) => {
                            error!("accept() failed: {:?}", err);
                        }
                    };
                }
                _ = cancel.cancelled() => {
                    break;
                }
            }
        }

        info!("Postgres watcher stopped");
        Ok(())
    }

    async fn conn_main(
        local_xact_tx: mpsc::Sender<XsMessage>,
        socket: tokio::net::TcpStream,
    ) -> anyhow::Result<()> {
        let mut handler = PgWatcherHandler { local_xact_tx };
        let pgbackend = PostgresBackendTCP::new(socket, AuthType::Trust, None)?;
        pgbackend
            .run(&mut handler, std::future::pending::<()>)
            .await?;
        Ok(())
    }
}

struct PgWatcherHandler {
    local_xact_tx: mpsc::Sender<XsMessage>,
}

#[async_trait::async_trait]
impl<IO> neon_postgres_backend::Handler<IO> for PgWatcherHandler
where
    IO: AsyncRead + AsyncWrite + Unpin + Sync + Send,
{
    fn startup(
        &mut self,
        _pgb: &mut PostgresBackend<IO>,
        _sm: &neon_pq_proto::FeStartupPacket,
    ) -> Result<(), QueryError> {
        Ok(())
    }

    async fn process_query(
        &mut self,
        pgb: &mut PostgresBackend<IO>,
        _query_string: &str,
    ) -> Result<(), QueryError> {
        // Switch to COPYBOTH
        pgb.write_message(&BeMessage::CopyBothResponse).await?;
        pgb.flush().await?;

        debug!("new postgres connection established");

        loop {
            let msg = pgb.read_message().await?;

            let copy_data_bytes = match msg {
                Some(FeMessage::CopyData(bytes)) => bytes,
                Some(FeMessage::Terminate) => break,
                Some(m) => {
                    return Err(QueryError::Other(anyhow::anyhow!(
                        "unexpected message: {m:?} during COPY",
                    )));
                }
                None => break, // client disconnected
            };

            let (commit_tx, commit_rx) = oneshot::channel();

            // Pass the transaction buffer to the xact manager
            self.local_xact_tx
                .send(XsMessage::LocalXact {
                    data: XactData::Encoded(copy_data_bytes),
                    commit_tx,
                })
                .await
                .map_err(|e| QueryError::Other(anyhow::anyhow!(e)))?;

            // Receive the commit/rollback data from the xact manager
            let rollback_info = commit_rx
                .await
                .map_err(|e| QueryError::Other(anyhow::anyhow!(e)))?;

            pgb.write_message(&BeMessage::CopyData(&serialize_rollback_info(
                rollback_info,
            )))
            .await?;
            pgb.flush().await?;
        }

        Ok(())
    }
}

fn serialize_rollback_info(info: Option<RollbackInfo>) -> Bytes {
    let mut buf = BytesMut::new();
    if let Some(info) = info {
        match info {
            RollbackInfo(by, RollbackReason::Db(db_err)) => {
                // Plus 1 to distinguish between region 0 and the end of message byte
                buf.put_u8((u32::from(by) + 1).try_into().unwrap());

                // Error message
                buf.put_slice(db_err.message.as_bytes());
                buf.put_u8(b'\0');

                // SQL error data
                buf.put_u8(1);
                buf.put_slice(&db_err.code);
                buf.put_slice(db_err.severity.as_bytes());
                buf.put_u8(b'\0');
                buf.put_slice(db_err.detail.as_bytes());
                buf.put_u8(b'\0');
                buf.put_slice(db_err.hint.as_bytes());
                buf.put_u8(b'\0');
            }
            RollbackInfo(by, RollbackReason::Other(err)) => {
                // Plus 1 to distinguish between region 0 and the end of message byte
                buf.put_u8((u32::from(by) + 1).try_into().unwrap());

                // Error message
                buf.put_slice(err.as_bytes());
                buf.put_u8(b'\0');
            }
        }
    }
    // End of message
    buf.put_u8(0);

    buf.freeze()
}
