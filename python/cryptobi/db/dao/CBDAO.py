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
from cryptobi.db.dao.cbmysql.CBMySQL import CBMySQL

class CBDAO:

    pool_size = 0
    _driver_counter = 0
    _drivers = []

    def __init__(_self):
        _self.pool_size = multiprocessing.cpu_count()
        _self.init_dao()

    def init_dao(_self):
        for i in range(_self.pool_size):
            _self._drivers.append(CBMySQL())

    def get_DAO(_self):
        ret = _self._drivers[_self._driver_counter]
        _self._driver_counter += 1
        return ret