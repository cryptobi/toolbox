"""

/*
 *
 * Crypto.BI Toolbox
 * https://Crypto.BI/
 *
 * Author: José Fonseca (self, https://zefonseca.com/)
 *
 * Distributed under the MIT software license, see the accompanying
 * file COPYING or http://www.opensource.org/licenses/mit-license.php.
 *
 */

"""

import sys
from cryptobi.db.dao.CBDAODriver import CBDAODriver
from cryptobi.model.graph.CBInfoNode import CBInfoNode
from cryptobi.model.blockchains.CBBlock import CBBlock
from cryptobi.model.blockchains.CBTx import CBTx
from cryptobi.model.blockchains.CBTx import CBTxIn
from cryptobi.model.blockchains.CBTx import CBTxOut
from cryptobi.model.blockchains.CBTx import CBTxOutAddress
from cryptobi.model.blockchains.CBBlockFile import CBBlockFile
from cryptobi.model.blockchains.CBBalance import CBBalance
from cryptobi.toolbox.system.CBConfig import CBConfig
from mysql import connector

class CBMySQL(CBDAODriver):

    """
        Implementation of CBDAODriver for MySQL RDBMS
    """

    def __init__(self):

        self.config = CBConfig.get_config()
        self.dbuser = self.config.get_conf("db.user".format(self.config.get_conf("db.db")))
        self.dbpass = self.config.get_conf("db.pass".format(self.config.get_conf("db.db")))
        self.dbdb   = self.config.get_conf("db.db".format(self.config.get_conf("db.db")))
        self.dbhost = self.config.get_conf("db.host".format(self.config.get_conf("db.db")))
        self.connect(self.dbuser, self.dbpass, self.dbhost, self.dbdb)

    def connect(self, u, passwd, h, db):
        self.cnx = connector.connect(user=u, password=passwd, host=h, database=db, use_pure=True)

    def test_connection(self) -> bool:
        """
        Send a trivial query to the server just to test connectivity.
        """

        ret = ""
        cursor = self.cnx.cursor()
        sql = "SELECT VERSION() AS _v"

        try:
            cursor.execute(sql)
        except Exception as e:
            print(e)
            cursor.close()
            return ""

        for version in cursor:
            cursor.close()
            return version

    def insert_block(self, block):
        """
        Block insertion currently is done in C++.
        """
        pass

    def get_block_by_hash(self, hash):
        """
        Parameter hash is a bytes instance.
        """
        return self.__get_block_by(hash, "hash_this_block")

    def get_next_block(self, prev_block_hash):
        """
        Parameter hash is a bytes instance.
        """
        return self.__get_block_by(prev_block_hash, "hash_prev_block")

    def __get_block_by(self, hash, column) -> CBBlock:
        """
        Refactored get block funcs by a certain column.
        """

        ret = None
        cursor = self.cnx.cursor()
        db = self.config.get_conf("db.db")
        sql = "SELECT table_seq, n_version, hash_this_block, hash_prev_block, hash_merkle_root, hash_next_block, n_time, n_bits, nonce, block_height FROM {}.cb_blockchain WHERE {} = %s".format(db, column)

        try:
            cursor.execute(sql, (hash, ))
        except Exception as e:
            print(e)
            cursor.close()
            return None

        for table_seq, n_version, hash_this_block, hash_prev_block, hash_merkle_root, hash_next_block, n_time, n_bits, nonce, block_height in cursor:
            cbb = CBBlock()
            cbb.table_seq = table_seq
            cbb.bits = n_bits
            cbb.hash = hash_this_block
            cbb.hash_next_block = hash_next_block
            cbb.hash_prev_block = hash_prev_block
            cbb.height = block_height
            cbb.merkle_root = hash_merkle_root
            cbb.n_version = n_version
            cbb.nonce = nonce
            cbb.ntime = n_time
            ret = cbb

        cursor.close()
        return ret

    def set_next_block_hash(self, thisblock_hash, nextblock_hash):
        cursor = self.cnx.cursor()
        db = self.config.get_conf("db.db")
        sql = "UPDATE {}.cb_blockchain SET hash_next_block = %s WHERE hash_this_block = %s".format(db)

        try:
            cursor.execute(sql, (nextblock_hash, thisblock_hash, ))
            self.cnx.commit()
        except Exception as e:
            print(e)
            cursor.close()
            return None

        cursor.close()

    def set_next_block_hash_and_height(self, thisblock_hash, nextblock_hash, height):
        cursor = self.cnx.cursor()
        db = self.config.get_conf("db.db")
        sql = "UPDATE {}.cb_blockchain SET hash_next_block = %s, block_height = %s WHERE hash_this_block = %s".format(db)

        try:
            cursor.execute(sql, (nextblock_hash, height, thisblock_hash, ))
            self.cnx.commit()
        except Exception as e:
            print(e)
            cursor.close()
            return None

        cursor.close()

    def get_latest_block(self):
        ret = None
        cursor = self.cnx.cursor()
        db = self.config.get_conf("db.db")
        sql = "SELECT table_seq, n_version, hash_this_block, hash_prev_block, hash_merkle_root, hash_next_block, n_time, n_bits, nonce, block_height FROM {}.cb_blockchain ORDER BY table_seq DESC LIMIT 1".format(db)

        try:
            cursor.execute(sql, (hash, ))
        except Exception as e:
            print(e)
            cursor.close()
            return None

        for table_seq, n_version, hash_this_block, hash_prev_block, hash_merkle_root, hash_next_block, n_time, n_bits, nonce, block_height in cursor:
            cbb = CBBlock()
            cbb.table_seq = table_seq
            cbb.bits = n_bits
            cbb.hash = hash_this_block
            cbb.hash_next_block = hash_next_block
            cbb.hash_prev_block = hash_prev_block
            cbb.height = block_height
            cbb.merkle_root = hash_merkle_root
            cbb.n_version = n_version
            cbb.nonce = nonce
            cbb.ntime = n_time
            ret = cbb

        cursor.close()
        return ret

    def get_latest_block_hash(self):
        block = self.get_latest_block()
        return block.hash

    def insert_block_file(self, filename, hash, byte_offset):
        """
        Block file insertion is currently done in C++.
        """
        pass

    def get_latest_block_file(self, filename, hash, byte_offset):
        ret = None
        cursor = self.cnx.cursor()
        db = self.config.get_conf("db.db")
        sql = "SELECT table_seq, hash_this_block, filename, byte_offset FROM {}.cb_blockchain_files ORDER BY table_seq DESC LIMIT 1".format(db)

        try:
            cursor.execute(sql, (hash, ))
        except Exception as e:
            print(e)
            cursor.close()
            return None

        for table_seq, hash_this_block, filename, byte_offset in cursor:
            cbb = CBBlockFile()
            cbb.table_seq = table_seq
            cbb.hash_this_block = hash_this_block
            cbb.filename = filename
            cbb.byte_offset = byte_offset
            ret = cbb

        cursor.close()
        return ret

    def insert_tx(self, tx):
        """
        TX insertion is currently done in C++.
        """
        pass

    def list_tx_by_block(self, block_hash):
        ret = []
        cursor = self.cnx.cursor()

        sql = "SELECT table_seq, hash_this_tx, n_version, has_witness, in_counter, lock_time, block_order, witness_hash, hash_block FROM {}.cb_tx WHERE hash_block = %s".format(self.config.get_conf("db.db"))
        cursor.execute(sql, (block_hash, ))

        for table_seq, hash_this_tx, n_version, has_witness, in_counter, lock_time, block_order, witness_hash, hash_block in cursor:
            tx = CBTx()
            tx.table_seq = table_seq
            tx.hash_this_tx = hash_this_tx
            tx.n_version = n_version
            tx.has_witness = has_witness
            tx.in_counter = in_counter
            tx.lock_time = lock_time
            tx.block_order = block_order
            tx.witness_hash = witness_hash
            tx.hash_block = hash_block
            ret.append(tx)

        cursor.close()
        return ret

    def get_tx_by_hash(self, hashtx):
        ret = None
        cursor = self.cnx.cursor()

        sql = "SELECT table_seq, hash_this_tx, n_version, has_witness, in_counter, lock_time, block_order, witness_hash, hash_block FROM {}.cb_tx WHERE hash_this_tx = %s".format(self.config.get_conf("db.db"))
        cursor.execute(sql, (hashtx, ))

        for table_seq, hash_this_tx, n_version, has_witness, in_counter, lock_time, block_order, witness_hash, hash_block in cursor:
            tx = CBTx()
            tx.table_seq = table_seq
            tx.hash_this_tx = hash_this_tx
            tx.n_version = n_version
            tx.has_witness = has_witness
            tx.in_counter = in_counter
            tx.lock_time = lock_time
            tx.block_order = block_order
            tx.witness_hash = witness_hash
            tx.hash_block = hash_block
            ret = tx

        cursor.close()
        return ret

    def get_latest_tx(self):
        ret = None
        cursor = self.cnx.cursor()

        sql = "SELECT table_seq, hash_this_tx, n_version, has_witness, in_counter, lock_time, block_order, witness_hash, hash_block FROM {}.cb_tx  ORDER BY table_seq DESC LIMIT 1".format(self.config.get_conf("db.db"))
        cursor.execute(sql)

        for table_seq, hash_this_tx, n_version, has_witness, in_counter, lock_time, block_order, witness_hash, hash_block in cursor:
            tx = CBTx()
            tx.table_seq = table_seq
            tx.hash_this_tx = hash_this_tx
            tx.n_version = n_version
            tx.has_witness = has_witness
            tx.in_counter = in_counter
            tx.lock_time = lock_time
            tx.block_order = block_order
            tx.witness_hash = witness_hash
            tx.hash_block = hash_block
            ret = tx

        cursor.close()
        return ret

    def insert_tx_in(self, tx):
        """
        TX input insertion is currently done in C++.
        """
        pass

    def get_tx_in(self, vin):
        ret = None
        cursor = self.cnx.cursor()

        sql = "SELECT table_seq, txid, n_vout, hash_this_tx, n_sequence FROM {}.cb_tx_in WHERE table_seq = %s".format(self.config.get_conf("db.db"))
        cursor.execute(sql, (vin.table_seq, ))

        for table_seq, txid, n_vout, hash_this_tx, n_sequence in cursor:
            ret = CBTxIn(table_seq, txid, n_vout, hash_this_tx, n_sequence)

        cursor.close()
        return ret

    def get_latest_tx_in(self):
        ret = None
        cursor = self.cnx.cursor()

        sql = "SELECT table_seq, txid, n_vout, hash_this_tx, n_sequence FROM {}.cb_tx_in ORDER BY table_seq DESC LIMIT 1".format(self.config.get_conf("db.db"))
        cursor.execute(sql)

        for table_seq, txid, n_vout, hash_this_tx, n_sequence in cursor:
            ret = CBTxIn(table_seq, txid, n_vout, hash_this_tx, n_sequence)

        cursor.close()
        return ret

    def list_tx_in(self, tx_hash):
        ret = []
        cursor = self.cnx.cursor()

        sql = "SELECT table_seq, txid, n_vout, hash_this_tx, n_sequence FROM {}.cb_tx_in WHERE hash_this_tx = %s".format(self.config.get_conf("db.db"))
        cursor.execute(sql, (tx_hash, ))

        for table_seq, txid, n_vout, hash_this_tx, n_sequence in cursor:
            ret.append(CBTxIn(table_seq, txid, n_vout, hash_this_tx, n_sequence))

        cursor.close()
        return ret

    def insert_tx_out(self, tx):
        """
        TX output insertion is currently done in C++.
        """
        pass

    def get_tx_out(self, vout):
        ret = None
        cursor = self.cnx.cursor()

        sql = "SELECT table_seq, satoshis, n_vout, hash_this_tx, hash_tx_spent, script_req_sigs, script_type FROM {}.cb_tx_out WHERE table_seq = %s".format(self.config.get_conf("db.db"))
        cursor.execute(sql, (vout.table_seq, ))

        for table_seq, satoshis, n_vout, hash_this_tx, hash_tx_spent, script_req_sigs, script_type in cursor:
            ret = CBTxOut(table_seq, satoshis, n_vout, hash_this_tx, hash_tx_spent, script_req_sigs, script_type)

        cursor.close()
        return ret

    def get_latest_tx_out(self):
        ret = None
        cursor = self.cnx.cursor()

        sql = "SELECT table_seq, satoshis, n_vout, hash_this_tx, hash_tx_spent, script_req_sigs, script_type FROM {}.cb_tx_out ORDER BY table_seq DESC LIMIT 1".format(self.config.get_conf("db.db"))
        cursor.execute(sql)

        for table_seq, satoshis, n_vout, hash_this_tx, hash_tx_spent, script_req_sigs, script_type in cursor:
            ret = CBTxOut(table_seq, satoshis, n_vout, hash_this_tx, hash_tx_spent, script_req_sigs, script_type)

        cursor.close()
        return ret

    def list_tx_out(self, tx_hash):
        ret = []
        cursor = self.cnx.cursor()

        sql = "SELECT table_seq, satoshis, n_vout, hash_this_tx, hash_tx_spent, script_req_sigs, script_type FROM {}.cb_tx_out WHERE hash_this_tx = %s".format(self.config.get_conf("db.db"))
        cursor.execute(sql, (tx_hash, ))

        for table_seq, satoshis, n_vout, hash_this_tx, hash_tx_spent, script_req_sigs, script_type in cursor:
            ret.append(CBTxOut(table_seq, satoshis, n_vout, hash_this_tx, hash_tx_spent, script_req_sigs, script_type))

        cursor.close()
        return ret

    def insert_tx_out_address(self, tx_hash, nvout, addr, req_sigs, script_type):
        """
        TX output address insertion is currently done in C++.
        """
        pass

    def list_tx_out_addresses(self, tx_hash):
        ret = []
        cursor = self.cnx.cursor()

        sql = "SELECT table_seq, n_vout, hash_tx, address, script_req_sigs, script_type FROM {}.cb_tx_out_addresses WHERE hash_tx = %s".format(self.config.get_conf("db.db"))
        cursor.execute(sql, (tx_hash, ))

        for table_seq, n_vout, hash_tx, address, script_req_sigs, script_type in cursor:
            ret.append(CBTxOutAddress(table_seq, n_vout, hash_tx, address, script_req_sigs, script_type))

        cursor.close()
        return ret

    def insert_address_graph(self, ag):
        """
        Inserts an address graph entry into table cb_address_graph or equivalent.
        @param ag is an instance of cryptobi.model.graph.CBInfoNode
        """
        # vin, vout, addr_from, addr_to, satoshis, n_time
        ret = -1
        cursor = self.cnx.cursor()

        sql = "INSERT INTO {}.cb_address_graph(n_vout, tx_from, n_vin, tx_to, address_from, address_to, satoshis, n_time) VALUES(%s, %s, %s, %s, %s, %s, %s, %s)".format(self.config.get_conf("db.db"))
        ret = cursor.execute(sql, (ag.n_vout, ag.tx_from, ag.n_vin, ag.tx_to, ag.address_from, ag.address_to, ag.satoshis, ag.n_time))
        ret = cursor.lastrowid
        cursor.close()
        return ret
        pass

    def list_address_graph(self, addr):
        ret = []
        cursor = self.cnx.cursor()

        sql = "SELECT cag_id, n_vout, tx_from, n_vin, tx_to, address_from, address_to, satoshis, n_time FROM {}.cb_address_graph WHERE address_from = %s OR address_to = %s".format(self.config.get_conf("db.db"))
        cursor.execute(sql, (addr, addr, ))

        for cin_id, block_hash, tx_hash, address, content in cursor:
            ret.append(CBInfoNode(cin_id, block_hash, tx_hash, address, content))

        cursor.close()
        return ret

    def set_address_balance(self, address, satoshis):
        """
        Update an address' balance in satoshis.
        @param satoshis is a BTC value in satoshi units.
        If the address does not exist, it is first created on the DB table.
        """
        ret = None
        cursor = self.cnx.cursor()

        sql = "REPLACE INTO {}.cb_address_balance(satoshis, address) VALUES(%s, %s)".format(self.config.get_conf("db.db"))
        ret = cursor.execute(sql, (satoshis, address, ))
        cursor.close()
        return ret

    def get_address_balance(self, address):
        ret = None
        cursor = self.cnx.cursor()

        sql = "SELECT cab_id, satoshis FROM {}.cb_address_balance WHERE address = %s".format(self.config.get_conf("db.db"))
        cursor.execute(sql, (address, ))

        for cab_id, satoshis in cursor:
            ret = CBBalance(cab_id, address, satoshis)

        cursor.close()
        return ret

    def insert_info_node(self, inode):
        """
        Insert an info node into cb_info_nodes table (or equivalent).
        @param inode is an instance of cryptobi.model.graph.CBInfoNode
        """
        ret = -1
        cursor = self.cnx.cursor()

        sql = "INSERT INTO {}.cb_info_nodes(block_hash, tx_hash, address, content) VALUES(%s, %s, %s, %s)".format(self.config.get_conf("db.db"))
        ret = cursor.execute(sql, (inode.block_hash, inode.tx_hash, inode.address, inode.content))
        ret = cursor.lastrowid
        cursor.close()
        return ret

    def get_info_node_by_id(self, id):
        ret = None
        cursor = self.cnx.cursor()

        sql = "SELECT block_hash, tx_hash, address, content FROM {}.cb_info_nodes WHERE cin_id = %s".format(self.config.get_conf("db.db"))
        cursor.execute(sql, (id, ))

        for block_hash, tx_hash, address, content in cursor:
            ret = CBInfoNode(id, block_hash, tx_hash, address, content)

        cursor.close()
        return ret

    def list_info_node_by_address(self, address):
        ret = []
        cursor = self.cnx.cursor()

        sql = "SELECT cin_id, block_hash, tx_hash, address, content FROM {}.cb_info_nodes WHERE address = %s".format(self.config.get_conf("db.db"))
        cursor.execute(sql, (address, ))

        for cin_id, block_hash, tx_hash, address, content in cursor:
            ret.append(CBInfoNode(cin_id, block_hash, tx_hash, address, content))

        cursor.close()
        return ret

    def list_info_node_by_block_hash(self,  hash):
        ret = []
        cursor = self.cnx.cursor()

        sql = "SELECT cin_id, block_hash, tx_hash, address, content FROM {}.cb_info_nodes WHERE block_hash = %s".format(self.config.get_conf("db.db"))
        cursor.execute(sql, (hash, ))

        for cin_id, block_hash, tx_hash, address, content in cursor:
            ret.append(CBInfoNode(cin_id, block_hash, tx_hash, address, content))

        cursor.close()
        return ret

    def list_info_node_by_tx_hash(self,  hash):
        ret = []
        cursor = self.cnx.cursor()

        sql = "SELECT cin_id, block_hash, tx_hash, address, content FROM {}.cb_info_nodes WHERE tx_hash = %s".format(self.config.get_conf("db.db"))
        cursor.execute(sql, (hash, ))

        for cin_id, block_hash, tx_hash, address, content in cursor:
            ret.append(CBInfoNode(cin_id, block_hash, tx_hash, address, content))

        cursor.close()
        return ret

    def list_info_nodes(self):
        ret = []
        cursor = self.cnx.cursor()

        sql = "SELECT cin_id, block_hash, tx_hash, address, content FROM {}.cb_info_nodes".format(self.config.get_conf("db.db"))
        cursor.execute(sql)

        for cin_id, block_hash, tx_hash, address, content in cursor:
            ret.append(CBInfoNode(cin_id, block_hash, tx_hash, address, content))

        cursor.close()
        return ret

    def search_info_node(self, q):
        ret = []
        cursor = self.cnx.cursor()

        sql = "SELECT cin_id, block_hash, tx_hash, address, content FROM {}.cb_info_nodes WHERE content LIKE %s".format(self.config.get_conf("db.db"))
        cursor.execute(sql, (q, ))

        for cin_id, block_hash, tx_hash, address, content in cursor:
            ret.append(CBInfoNode(cin_id, block_hash, tx_hash, address, content))

        cursor.close()
        return ret

    def update_info_node(self, inode):

        """
        Update an info node with info contained in @param inode
        @param inode is an instance of cryptobi.model.graph.CBInfoNode
               and must contain cin_id > 0
        """

        ret = -1
        cursor = self.cnx.cursor()

        sql = "UPDATE {}.cb_info_nodes SET block_hash = %s, tx_hash = %s, address = %s, content = %s WHERE cin_id = %s".format(self.config.get_conf("db.db"))
        ret = cursor.execute(sql, (inode.block_hash, inode.tx_hash, inode.address, inode.content, inode.cin_id, ))
        cursor.close()
        return ret

    def delete_info_node(self, id):
        """
        Insert an info node into cb_info_nodes table (or equivalent).
        @param inode is an instance of cryptobi.model.graph.CBInfoNode
        """
        ret = -1
        cursor = self.cnx.cursor()

        sql = "DELETE FROM {}.cb_info_nodes WHERE cin_id = %s".format(self.config.get_conf("db.db"))
        ret = cursor.execute(sql, (id, ))
        ret = cursor.lastrowid
        cursor.close()
        return ret

    def disable_keys(self):
        tables = self.list_tables()
        cursor = self.cnx.cursor()

        for table in tables:
            cursor.execute("ALTER TABLE {} DISABLE KEYS".format(table))

        cursor.close()

    def enable_keys(self):
        tables = self.list_tables()
        cursor = self.cnx.cursor()

        for table in tables:
            cursor.execute("ALTER TABLE {} ENABLE KEYS".format(table))

        cursor.close()

    def list_tables(self):
        ret = []
        cursor = self.cnx.cursor()

        cursor.execute("USE {}".format(self.config.get_conf("db.db")))
        cursor.execute("SHOW TABLES")

        for table, in cursor:
            ret.append(table)

        cursor.close()
        return ret
