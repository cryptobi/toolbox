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

    def test_getnextblock(self):
        genesis = CBBlock.genesis()
        nb = self.mq.get_next_block(genesis.hash)
        self.assertIsNot(nb, None)

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

