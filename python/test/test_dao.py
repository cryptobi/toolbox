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
import multiprocessing
from cryptobi.db.dao.CBDAO import CBDAO

class CBDAOTest(unittest.TestCase):

    def setUp(self):
        self.dao = CBDAO.get_DAO()

    def test_getdao(self):
        self.assertIsNot(self.dao, None)

    def test_poolsize(self):
        c = CBDAO()
        self.assertEquals(c.pool_size, multiprocessing.cpu_count())
