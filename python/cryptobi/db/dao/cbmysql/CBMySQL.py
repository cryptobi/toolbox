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
from mysql import connector
from cryptobi.db.dao.CBDAODriver import CBDAODriver
from cryptobi.model.graph.CBInfoNode import CBInfoNode
from cryptobi.model.blockchains.CBBlock import CBBlock
from cryptobi.toolbox.system.CBConfig import CBConfig

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
        self.cnx = connector.connect(user=u, password=passwd, host=h, database=db)

    def test_connection(self):
        pass

    def insert_block(self, block):
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

    def __get_block_by(self, hash, column):
        """
        Refactored get block funcs by a certain column.
        """
        ret = []
        cursor = self.cnx.cursor()

        sql = ("SELECT 	table_seq, n_version, hash_this_block, hash_prev_block, hash_merkle_root, hash_next_block, n_time, n_bits, nonce, block_height FROM {}.cb_address_graph WHERE hash_this_block = %s".format(self.config.get_conf("db.db")))
        cursor.execute(sql, (hash, ))

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
            ret.append(cbb)

        cursor.close()
        return ret

    def get_latest_block(self):
        pass

    def get_latest_block_hash(self):
        pass

    def insert_block_file(self, filename, hash, byte_offset):
        pass

    def get_latest_block_file(self, filename, hash, byte_offset):
        pass

    def insert_tx(self, tx):
        pass

    def get_tx_by_hash(self, hash):
        pass

    def get_latest_tx(self):
        pass

    def insert_tx_in(self, tx):
        pass

    def get_tx_in(self, vin):
        pass

    def get_latest_tx_in(self):
        pass

    def list_tx_in(self, tx_hash):
        ret = []
        return ret

    def insert_tx_out(self, tx):
        pass

    def get_tx_out(self, vout):
        pass

    def get_latest_tx_out(self):
        pass

    def list_tx_out(self, tx_hash):
        ret = []
        return ret

    def insert_tx_out_address(self, tx_hash, nvout, addr, req_sigs, script_type):
        pass

    def list_tx_out_addresses(self, tx_hash):
        pass

    def insert_address_graph(self, vin, vout, addr_from, addr_to, satoshis, n_time):
        pass

    def list_address_graph(self, addr):
        ret = []
        cursor = self.cnx.cursor()

        sql = ("SELECT cag_id, n_vout, tx_from, n_vin, tx_to, address_from, address_to, satoshis, n_time FROM {}.cb_address_graph WHERE address_from = %s OR address_to = %s".format(self.config.get_conf("db.db")))
        cursor.execute(sql, (addr, addr, ) )

        for cin_id, block_hash, tx_hash, address, content in cursor:
            ret.append(CBInfoNode(cin_id, block_hash, tx_hash, address, content))

        cursor.close()
        return ret

    def insert_info_node(self, inode):
        pass

    def get_info_node_by_id(self, id):
        ret = None
        cursor = self.cnx.cursor()

        sql = ("SELECT cin_id, HEX(block_hash), HEX(tx_hash), address, content FROM {}.cb_info_nodes WHERE block_hash = %s".format(self.config.get_conf("db.db")))
        cursor.execute(sql, (hash, ) )

        for cin_id, block_hash, tx_hash, address, content in cursor:
            ret = CBInfoNode(cin_id, block_hash, tx_hash, address, content)

        cursor.close()
        return ret

    def list_info_node_by_address(self, address):
        ret = []
        cursor = self.cnx.cursor()

        sql = ("SELECT cin_id, HEX(block_hash), HEX(tx_hash), address, content FROM {}.cb_info_nodes WHERE address = %s".format(self.config.get_conf("db.db")))
        cursor.execute(sql, (address, ))

        for cin_id, block_hash, tx_hash, address, content in cursor:
            ret.append(CBInfoNode(cin_id, block_hash, tx_hash, address, content))

        cursor.close()
        return ret

    def list_info_node_by_block_hash(self,  hash):
        ret = []
        cursor = self.cnx.cursor()

        sql = ("SELECT cin_id, HEX(block_hash), HEX(tx_hash), address, content FROM {}.cb_info_nodes WHERE block_hash = %s".format(self.config.get_conf("db.db")))
        cursor.execute(sql, (hash, ) )

        for cin_id, block_hash, tx_hash, address, content in cursor:
            ret.append(CBInfoNode(cin_id, block_hash, tx_hash, address, content))

        cursor.close()
        return ret

    def list_info_node_by_tx_hash(self,  hash):
        ret = []
        cursor = self.cnx.cursor()

        sql = ("SELECT cin_id, HEX(block_hash), HEX(tx_hash), address, content FROM {}.cb_info_nodes WHERE tx_hash = %s".format(self.config.get_conf("db.db")))
        cursor.execute(sql, (hash, ) )

        for cin_id, block_hash, tx_hash, address, content in cursor:
            ret.append(CBInfoNode(cin_id, block_hash, tx_hash, address, content))

        cursor.close()
        return ret

    def list_info_nodes(self):
        ret = []
        cursor = self.cnx.cursor()

        sql = ("SELECT cin_id, HEX(block_hash), HEX(tx_hash), address, content FROM {}.cb_info_nodes".format(self.config.get_conf("db.db")))
        cursor.execute(sql)

        for cin_id, block_hash, tx_hash, address, content in cursor:
            ret.append(CBInfoNode(cin_id, block_hash, tx_hash, address, content))

        cursor.close()
        return ret

    def search_info_node(self, q):
        pass

    def update_info_node(self, inode):
        pass

    def delete_info_node(self, id):
        pass

    def disable_keys(self):
        pass

    def enable_keys(self):
        pass

