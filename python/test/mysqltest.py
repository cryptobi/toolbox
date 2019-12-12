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

class CBDBConnTests(unittest.TestCase):

    def setUp(self):
        self.mq = CBMySQL()

    def test_getnextblock(self):
        genesis = CBBlock()
        nb = self.mq.get_next_block(genesis.hash)
        self.assertIsNot(nb, None)

    def test_getblockbyhash(self):
        genesis = CBBlock()
        nb = self.mq.get_block_by_hash(genesis.hash)
        self.assertEquals(nb.hash, genesis.hash)

