use anyhow::{bail, Context};
use bytes::{Buf, Bytes};
use std::mem::size_of;

type Oid = u32;

#[derive(Debug)]
pub struct Transaction {
    dbid: Oid,
    xid: u32,
    relations: Vec<Relation>,
}

impl Transaction {
    pub fn parse(buf: &mut Bytes) -> anyhow::Result<Transaction> {
        const HEADER_SZ: usize = size_of::<Oid>() + size_of::<u32>() + size_of::<u32>();

        if buf.remaining() < HEADER_SZ {
            bail!(
                "header too short, expected: {}, remaining: {}",
                HEADER_SZ,
                buf.remaining()
            );
        }
        let dbid = buf.get_u32();
        let xid = buf.get_u32();
        let readlen = buf.get_u32() as usize;

        if buf.remaining() < readlen {
            bail!(
                "read section too short, expected: {}, remaining: {}",
                readlen,
                buf.remaining()
            );
        }
        let mut readbuf = buf.split_to(readlen);
        let mut relations = vec![];
        let mut i = 0;
        while readbuf.has_remaining() {
            let ctx = || {
                let i = i;
                format!(
                    "failed to parse relation {} [dbid: {}, xid: {}, readlen: {}]",
                    i, dbid, xid, readlen
                )
            };
            let r = Relation::parse(&mut readbuf).with_context(ctx)?;
            relations.push(r);
            i += 1;
        }

        Ok(Transaction {
            dbid,
            xid,
            relations,
        })
    }
}

#[derive(Debug)]
pub enum Relation {
    Table {
        oid: u32,
        csn: u32,
        tuples: Vec<Tuple>,
    },
    Index {
        oid: u32,
        pages: Vec<Page>,
    },
}

#[derive(Debug)]
pub struct Tuple {
    blocknum: u32,
    offset: u16,
}

#[derive(Debug)]
pub struct Page {
    blocknum: u32,
    csn: u32,
}

impl Relation {
    pub fn parse(buf: &mut Bytes) -> anyhow::Result<Relation> {
        let rel_type = buf.get_u8();
        match rel_type {
            b'T' => Relation::parse_table(buf),
            b'I' => Relation::parse_index(buf),
            _ => bail!("invalid relation type: {}", rel_type),
        }
    }

    fn parse_table(buf: &mut Bytes) -> anyhow::Result<Relation> {
        const TABLE_HEADER_SZ: usize = size_of::<Oid>() + size_of::<u32>() + size_of::<u32>();

        if buf.remaining() < TABLE_HEADER_SZ {
            bail!(
                "table header length too short, expected: {}, remaining: {}",
                TABLE_HEADER_SZ,
                buf.remaining()
            );
        }
        let relid = buf.get_u32();
        let ntuples = buf.get_u32();
        let csn = buf.get_u32();
        let mut tuples = vec![];
        for i in 0..ntuples {
            let ctx = || {
                let i = i;
                format!(
                    "failed to parse tuple {} in Table(relid: {}, ntuples: {}, csn: {})",
                    i, relid, ntuples, csn
                )
            };
            let t = Relation::parse_tuple(buf).with_context(ctx)?;
            tuples.push(t);
        }

        Ok(Relation::Table {
            oid: relid,
            csn,
            tuples,
        })
    }

    fn parse_tuple(buf: &mut Bytes) -> anyhow::Result<Tuple> {
        const TUPLE_SZ: usize = size_of::<u32>() + size_of::<u16>();

        if buf.remaining() < TUPLE_SZ {
            bail!(
                "tuple too short, expected: {}, remaining: {}",
                TUPLE_SZ,
                buf.remaining()
            );
        }
        let blocknum = buf.get_u32();
        let offset = buf.get_u16();

        Ok(Tuple { blocknum, offset })
    }

    fn parse_index(buf: &mut Bytes) -> anyhow::Result<Relation> {
        const INDEX_HEADER_SZ: usize = size_of::<Oid>() + size_of::<u32>();

        if buf.remaining() < INDEX_HEADER_SZ {
            bail!(
                "table header length too short, expected: {}, remaining: {}",
                INDEX_HEADER_SZ,
                buf.remaining()
            );
        }
        let relid = buf.get_u32();
        let npages = buf.get_u32();
        let mut pages = vec![];
        for i in 0..npages {
            let ctx = || {
                let i = i;
                format!(
                    "failed to parse page {} in Index(relid: {}, npages: {})",
                    i, relid, npages
                )
            };
            let p = Relation::parse_page(buf).with_context(ctx)?;
            pages.push(p);
        }

        Ok(Relation::Index { oid: relid, pages })
    }

    fn parse_page(buf: &mut Bytes) -> anyhow::Result<Page> {
        const PAGE_SZ: usize = size_of::<u32>() + size_of::<u32>();

        if buf.remaining() < PAGE_SZ {
            bail!(
                "page too short, expected: {}, remaining: {}",
                PAGE_SZ,
                buf.remaining()
            );
        }
        let blocknum = buf.get_u32();
        let csn = buf.get_u32();

        Ok(Page { blocknum, csn })
    }
}
