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

import multiprocessing
import random
from cryptobi.db.dao.cbmysql.CBMySQL import CBMySQL

class CBDAO:

    _dao = None

    def __init__(self):
        self._drivers = []
        self.pool_size = multiprocessing.cpu_count()
        self.init_dao()

    def init_dao(self):
        for i in range(self.pool_size):
            ix = CBMySQL()
            self._drivers.append(ix)

    @staticmethod
    def get_DAO():

        if CBDAO._dao is None:
            CBDAO._dao = CBDAO()
        return random.choice(CBDAO._dao._drivers)
