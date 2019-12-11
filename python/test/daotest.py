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
from cryptobi.db.dao.CBDAO import CBDAO

class CBDAOTest(unittest.TestCase):

    def setUp(self):
        dao_a = CBDAO()
        self.dao = dao_a.get_DAO()

    def test_connect(self):
        return self.dao.test_connection()