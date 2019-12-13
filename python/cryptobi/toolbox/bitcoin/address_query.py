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


 A Satoshi Address: 1A1zP1eP5QGefi2DMPTfTL5SLmv7DivfNa

"""

# KNOWN BUG: must load the mysql module before all other libraries
# so they don't load an incompatible version of openssl which causes
# random segfaults with mysql procedures
# mysql module is not used here but must be loaded first on some systems
from mysql import connector

from cryptobi.toolbox.system.CBConfig import CBConfig
import cryptobi.model.graph.CBAGNode
from cryptobi.db.dao.CBDAO import CBDAO
import sys

config = CBConfig.get_config()

dao_a = CBDAO()
dao = dao_a.get_DAO()

addr = config.get_conf("listargs")

# find all infonode texts
infonodes = dao.list_info_node_by_address(addr)

for ifn in infonodes:
    print(ifn)

# find all TX's from this address
graph = dao.list_address_graph(addr)

for ifn in graph:
    print(ifn)

# find final balance
# TODO