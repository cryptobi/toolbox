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

    Enable database keys.
    This might place the DBMS system under heavy load while building keys.

"""

# KNOWN BUG: must load the mysql module before all other libraries
# so they don't load an incompatible version of openssl which causes
# random segfaults with mysql procedures
# mysql module is not used here but must be loaded first on some systems
from mysql import connector

from cryptobi.toolbox.system.CBConfig import CBConfig
from cryptobi.db.dao.CBDAO import CBDAO

config = CBConfig.get_config()
dao = CBDAO.get_DAO()

dao.enable_keys()
