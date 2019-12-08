drop database cryptobitoolbox_bitcoin;

-- Crypto.BI Toolbox (https://Crypto.BI/)
-- Author: José Fonseca 2019-10-03 (https://zefonseca.com)
-- Copyright (C) 2019 - José Fonseca
-- See LICENSE for licensing details.

-- Bitcoin-specific blockchain schema

create database if not exists cryptobitoolbox_bitcoin;

use cryptobitoolbox_bitcoin;

create table cb_blockchain(
	table_seq bigint NOT NULL AUTO_INCREMENT,
    n_version int not null default 0,
    hash_this_block char(32) BINARY,
    hash_prev_block char(32) BINARY not null default X'0000000000000000000000000000000000000000000000000000000000000000',
    hash_merkle_root char(32) BINARY UNIQUE not null default X'0000000000000000000000000000000000000000000000000000000000000000',
    hash_next_block char(32) BINARY default null, -- not standard, optional
    n_time int not null default 0,
    n_bits int not null default 0,
    nonce int not null default 0,
    block_height int not null default 0,
    PRIMARY KEY by_table_seq USING BTREE (table_seq),
    KEY by_hash_this_block USING HASH (hash_this_block),
    KEY by_hash_prev_block USING HASH (hash_prev_block),
    KEY by_hash_merkle_root USING HASH (hash_merkle_root),
    KEY by_hash_next_block USING HASH (hash_next_block),
    KEY by_n_time USING BTREE (n_time),
    KEY by_n_bits USING BTREE (n_bits),
    KEY by_nonce USING BTREE (nonce),
    KEY by_n_version USING BTREE (n_version),
    KEY by_block_height USING BTREE (block_height)
) CHARACTER SET binary ENGINE=MyISAM;

create table cb_blockchain_files(
	table_seq bigint NOT NULL AUTO_INCREMENT,
    hash_this_block char(32) BINARY,
    filename varchar(256) not null,
    byte_offset int default null,
    PRIMARY KEY by_table_seq USING BTREE (table_seq),
    KEY by_hash_this_block USING HASH (hash_this_block),
    KEY by_filename USING HASH (filename)
) CHARACTER SET binary ENGINE=MyISAM;

create table cb_tx(
	table_seq bigint NOT NULL AUTO_INCREMENT,
    hash_this_tx char(32) BINARY,
    n_version int not null default 1,
    has_witness tinyint(1) not null,
    in_counter int not null,
    out_counter int not null,
    lock_time int not null default 0,
    block_order int not null default 0, -- order of this tx within the block
    witness_hash char(32) BINARY not null default X'0000000000000000000000000000000000000000000000000000000000000000',
    hash_block char(32) BINARY not null default X'0000000000000000000000000000000000000000000000000000000000000000',
    PRIMARY KEY by_table_seq USING BTREE (table_seq),
    KEY by_hash_this_tx USING HASH (hash_this_tx),
    KEY by_has_witness USING HASH (has_witness),
    KEY by_witness_hash USING HASH (witness_hash),
    KEY by_hash_block USING HASH (hash_block),
    KEY by_block_order USING BTREE (block_order),
    KEY by_lock_time USING BTREE (lock_time),
    KEY by_n_version USING BTREE (n_version)
) CHARACTER SET binary ENGINE=MyISAM;

create table cb_tx_in(
	table_seq bigint NOT NULL AUTO_INCREMENT,
    txid char(32) BINARY, -- the vout TX
    n_vout int not null,
    hash_this_tx char(32) BINARY, -- the TX this vin is on
    n_sequence bigint not null,
    PRIMARY KEY by_table_seq USING BTREE (table_seq),
    KEY by_hashTxN USING HASH (hash_this_tx, n_sequence),
    KEY by_n_sequence USING BTREE (n_sequence),
    KEY by_txid(txid),
	KEY by_nvout(n_vout)
)CHARACTER SET binary ENGINE=MyISAM;

create table cb_tx_out(
	table_seq bigint NOT NULL AUTO_INCREMENT,
    satoshis bigint not null,
    n_vout int not null,
    hash_this_tx char(32) BINARY, -- the TX this vout is on
    hash_tx_spent char(32) BINARY DEFAULT NULL, -- if not null, tx is spent at hash_tx_spent (optimization)
    script_req_sigs int not null default 1,
    script_type tinyint, -- 0 pubkeyhash, 1 scripthash, 2 nonstandard
    PRIMARY KEY by_table_seq USING BTREE (table_seq),
    KEY by_hash_tx_prevout USING HASH (hash_this_tx, n_vout),
    KEY by_satoshis USING BTREE (satoshis),
    KEY by_hash_tx_spent USING HASH (hash_tx_spent),
    KEY by_script_req_sigs USING BTREE (script_req_sigs),
    KEY by_script_type USING BTREE (script_type)
)CHARACTER SET binary ENGINE=MyISAM;

create table cb_tx_out_addresses(
	table_seq bigint NOT NULL AUTO_INCREMENT,
    n_vout int not null,
    hash_tx char(32) BINARY, -- the TX this vout is on
    address varchar(90) NOT NULL,
    script_req_sigs int not null default 1,
    script_type tinyint, -- 0 pubkeyhash, 1 scripthash, 2 nonstandard, 3 nulldata
    PRIMARY KEY by_table_seq USING BTREE (table_seq),
    KEY by_hashTxPrevout USING HASH (hash_tx, n_vout),
    KEY by_address USING HASH (address),
    KEY by_script_req_sigs USING BTREE(script_req_sigs),
    KEY by_script_type USING HASH(script_type)
)CHARACTER SET binary ENGINE=MyISAM;

create table cb_address_graph(
    cag_id bigint NOT NULL AUTO_INCREMENT,
    n_vout int not null,
    tx_from char(32) BINARY, -- the TX the vout is in
    n_vin int not null,
    tx_to char(32) BINARY, -- the TX the vin is in
    address_from varchar(90) NOT NULL, -- 90 char max bech32 address len
    address_to varchar(90) NOT NULL,
    satoshis bigint not null,
    n_time int not null default 0, -- copied from the block
    PRIMARY KEY by_cag_id USING BTREE (cag_id),
    KEY by_satoshis USING BTREE (satoshis),
    KEY by_n_time USING BTREE (n_time),
    KEY by_adress_from USING HASH (address_from),
    KEY by_address_to USING HASH (address_to),
    KEY by_tx_from  USING HASH (tx_from),
    KEY by_tx_to  USING HASH (tx_to),
    KEY by_n_vout USING BTREE (n_vout),
    KEY by_n_vin USING BTREE (n_vin)
) CHARACTER SET binary ENGINE=MyISAM;

-- this is where we save the information we have
-- about certain nodes (tx, blocks, etc)
create table cb_info_nodes(
    cin_id bigint NOT NULL AUTO_INCREMENT,
    block_hash char(32) BINARY,
    tx_hash char(32) BINARY,
    address varchar(90),
    content text,
    PRIMARY KEY by_cin_id USING BTREE (cin_id),
    KEY by_block_hash USING HASH (block_hash),
    KEY by_tx_hash USING HASH (tx_hash),
    KEY by_address USING HASH (address),
    KEY by_content USING BTREE (content(1024))
)  CHARACTER SET utf8 ENGINE=InnoDB;

-- INSERT GENESIS BLOCK
INSERT INTO cb_blockchain(n_version, hash_this_block, hash_prev_block,hash_merkle_root,hash_next_block, n_time, n_bits, nonce, block_height)
VALUES(1, X'000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f', X'0000000000000000000000000000000000000000000000000000000000000000',
       X'4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b', X'00000000839a8e6886ab5951d76f411475428afc90947ee320161bbf18eb6048',
       1231006505, 486604799, 2083236893, 0 );

