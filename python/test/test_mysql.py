"""

/*
 *
 * Crypto.BI Toolbox
 * https://Crypto.BI/
 *
 * Author: José Fonseca (https://zefonseca.com/)
 *
 * Distributed under the MIT software license, see the accompanying
 * file COPYING or http://www.opensource.org/licenses/mit-license.php.
 *
 */

"""

import unittest
from cryptobi.db.dao.cbmysql.CBMySQL import CBMySQL
from cryptobi.model.blockchains.CBBlock import CBBlock
from cryptobi.model.graph.CBInfoNode import CBInfoNode

class CBDBConnTests(unittest.TestCase):

    def setUp(self):
        # these are real hashes but the data is invalid, for testing purposes only
        self.test_address = "1A1zP1eP5QGefi2DMPTfTL5SLmv7DivfNa"
        self.test_block = bytes.fromhex("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f")
        self.test_tx = bytes.fromhex("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b")
        self.test_content = "ajklsdf9asdf9as9d8f89 asdf89as9 8df89as9d8f98asd 98f98a sd98f9a8 sdf89a89sdf98a98 sdf89a89s f89a98sdf89a89  sdf98ads98fa89sdf89asd89f89asd"
        self.test_amount = 1234567890
        self.inode = CBInfoNode(0, self.test_block, self.test_tx, self.test_address, self.test_content)
        self.mq = CBMySQL()

    def test_test(self):
        self.assertTrue(self.mq.test_connection())

    def test_getsetnextblock(self):
        genesis = CBBlock.genesis()
        nb = self.mq.get_next_block(genesis.hash)
        self.assertIsNot(nb, None)
        self.mq.set_next_block_hash(genesis.hash, nb.hash)
        genesis2 = self.mq.get_block_by_hash(genesis.hash)
        self.assertEquals(nb.hash, genesis2.hash_next_block)

    def test_snbhah(self):
        genesis = CBBlock.genesis()
        nb = self.mq.get_next_block(genesis.hash)
        self.assertIsNot(nb, None)
        self.assertEquals(nb.height, 1)
        self.mq.set_next_block_hash_and_height(genesis.hash, nb.hash, 1)
        genesis2 = self.mq.get_block_by_hash(genesis.hash)
        self.assertEquals(nb.hash, genesis2.hash_next_block)
        block1 = self.mq.get_block_by_hash(nb.hash)
        self.assertEquals(block1.height, 1)

    def test_getblockbyhash(self):
        genesis = CBBlock.genesis()
        nb = self.mq.get_block_by_hash(genesis.hash)
        self.assertEquals(nb.hash, genesis.hash)

    def test_addrbalance(self):
        self.mq.set_address_balance(self.test_address, self.test_amount)
        balance = self.mq.get_address_balance(self.test_address)
        self.assertEquals(self.test_amount, balance.satoshis)

    def test_infonode(self):
        id = self.mq.insert_info_node(self.inode)
        self.assertGreater(id, 0)
        self.inode.cin_id = id
        inode = self.mq.get_info_node_by_id(id)
        self.assertEquals(inode.address, self.test_address)
        self.assertEquals(inode.block_hash, self.test_block)
        self.assertEquals(inode.tx_hash, self.test_tx)
        self.assertEquals(inode.content, self.test_content)

    def test_getlatestblock(self):
        block = self.mq.get_latest_block()
        self.assertIsNot(block, None)
        hsh = self.mq.get_latest_block_hash()
        self.assertIsNot(hsh, None)

    def test_getlatestblock_file(self):
        fil = self.mq.get_latest_block_file()
        self.assertIsNot(fil, None)

    def test_list_tx_by_block(self):
        genesis = CBBlock.genesis()
        nb = self.mq.get_next_block(genesis.hash)
        self.assertIsNot(nb, None)
        self.assertEquals(nb.height, 1)
        txs = self.mq.list_tx_by_block(nb.hash)
        self.assertTrue(len(txs) > 0)
        tx = txs[0]

    def test_list_tx_by_block(self):
        genesis = CBBlock.genesis()
        nb = self.mq.get_next_block(genesis.hash)
        self.assertIsNot(nb, None)
        self.assertEquals(nb.height, 1)
        txs = self.mq.list_tx_by_block(nb.hash)
        self.assertTrue(len(txs) > 0)
        tx = txs[0]

    def test_get_tx_by_hash(self):
        genesis = CBBlock.genesis()
        nb = self.mq.get_next_block(genesis.hash)
        self.assertIsNot(nb, None)
        self.assertEquals(nb.height, 1)
        txs = self.mq.list_tx_by_block(nb.hash)
        self.assertTrue(len(txs) > 0)
        tx = txs[0]
        tx2 = self.mq.get_tx_by_hash(tx.hash_this_tx)
        self.assertEquals(tx2.hash_this_tx, tx.hash_this_tx)

    def test_get_latest_tx(self):
        tx = self.mq.get_latest_tx()
        self.assertIsNot(tx, None)

    def test_get_latest_tx_in(self):
        txin = self.mq.get_latest_tx_in()
        self.assertIsNot(txin, None)
        txin2 = self.mq.get_tx_in(txin)
        self.assertIsNot(txin2, None)
        self.assertEquals(txin2.txid, txin.txid)
        self.assertEquals(txin2.hash_this_tx, txin.hash_this_tx)
        lstxin = self.mq.list_tx_in(txin.hash_this_tx)
        self.assertTrue(len(lstxin) > 0)

    def test_get_latest_tx_out(self):
        txout = self.mq.get_latest_tx_out()
        self.assertIsNot(txout, None)
        txout2 = self.mq.get_tx_out(txout)
        self.assertIsNot(txout2, None)
        self.assertEquals(txout2.hash_this_tx, txout.hash_this_tx)
        lstxout = self.mq.list_tx_out(txout.hash_this_tx)
        self.assertTrue(len(lstxout) > 0)

    def test_list_tx_out_addresses(self):
        txout = self.mq.get_latest_tx_out()
        self.assertIsNot(txout, None)
        lsaddr = self.mq.list_tx_out_addresses(txout.hash_this_tx)
        self.assertTrue(len(lsaddr) > 0)

    def test_list_address_graph(self):
        txout = self.mq.get_latest_tx_out()
        self.assertIsNot(txout, None)
        lsaddr = self.mq.list_tx_out_addresses(txout.hash_this_tx)
        self.assertTrue(len(lsaddr) > 0)
        addr = lsaddr[0]
        lsg = self.mq.list_address_graph(addr.address)
        self.assertTrue(len(lsg) > 0)

    def test_set_address_balance(self):
        test_addr = "text_" + self.test_address
        self.mq.set_address_balance(test_addr, self.test_amount)
        bal = self.mq.get_address_balance(test_addr)
        self.assertIsNot(bal, None)
        self.assertTrue(bal.satoshis == self.test_amount)
        self.assertTrue(bal.address == test_addr)

    def test_info_node(self):
        inode = CBInfoNode(0, self.test_block, self.test_tx, self.test_address, self.test_content)
        inode_id = self.mq.insert_info_node(inode)
        self.assertTrue(inode_id > 0)
        inode2 = self.mq.get_info_node_by_id(inode_id)
        self.assertIsNot(inode2, None)
        self.assertEquals(inode2.address, inode.address)
        self.assertEquals(inode2.tx_hash, inode.tx_hash)
        self.assertEquals(inode2.block_hash, inode.block_hash)
        self.assertEquals(inode2.content, inode.content)
        lsaddr = self.mq.list_info_node_by_address(self.test_address)
        self.assertTrue(len(lsaddr) > 0)
        lsbh = self.mq.list_info_node_by_block_hash(self.test_block)
        self.assertTrue(len(lsbh) > 0)
        lstx = self.mq.list_info_node_by_tx_hash(self.test_tx)
        self.assertTrue(len(lstx) > 0)
        ls = self.mq.list_info_nodes()
        self.assertTrue(len(ls) > 0)
        lss = self.mq.search_info_node(self.test_content)
        self.assertTrue(len(lss) > 0)
        inode2.content = "new_" + inode2.content
        self.mq.update_info_node(inode2)
        inode3 = self.mq.get_info_node_by_id(inode2.cin_id)
        self.assertIsNot(inode3, None)
        self.assertEquals(inode3.content, inode2.content)
        self.mq.delete_info_node(inode2.cin_id)
        inode4 = self.mq.get_info_node_by_id(inode2.cin_id)
        self.assertIs(inode4, None)

    def test_list_tables(self):
        ls = self.mq.list_tables()
        self.assertTrue(len(ls) > 0)

