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

import cryptobi.toolbox.system.CBConfig
import cryptobi.model.graph.CBAGNode
from cryptobi.db.dao.CBDAO import CBDAO
import argparse


parser = argparse.ArgumentParser(description = "Query a Bitcoin address from the local databse.")
parser.add_argument('bitcoin_address', type=str, nargs=1)
args = parser.parse_args()

dao_a = CBDAO()
dao = dao_a.get_DAO()

addr = args.bitcoin_address
print(addr)

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