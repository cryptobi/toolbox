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

class CBDBConnTests(unittest.TestCase):

    def setUp(self):
        self.mq = CBMySQL()

