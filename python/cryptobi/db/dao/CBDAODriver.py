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

class CBDAODriver:
    """
        Abstract class / interface for database access layer.
    """
    def connect(self, user, password, host, db):
        pass

    def test_connection(self):
        pass

    def insert_block(self, block):
        pass

    def get_block_by_hash(self, hash):
        pass

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
        pass

    def insert_tx_out(self, tx):
        pass

    def get_tx_out(self, vout):
        pass

    def get_latest_tx_out(self):
        pass

    def list_tx_out(self, tx_hash):
        pass

    def insert_tx_out_address(self, tx_hash, nvout, addr, req_sigs, script_type):
        pass

    def list_tx_out_addresses(self, tx_hash):
        pass

    def insert_address_graph(self, vin, vout, addr_from, addr_to, satoshis, n_time):
        pass

    def list_address_graph(self, addr):
        pass

    def insert_info_node(self, inode):
        pass

    def get_info_node_by_id(self, id):
        pass

    def list_info_node_by_address(self, address):
        pass

    def list_info_node_by_block_hash(self, hash):
        pass

    def list_info_node_by_tx_hash(self, hash):
        pass

    def list_info_nodes(self):
        pass

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

