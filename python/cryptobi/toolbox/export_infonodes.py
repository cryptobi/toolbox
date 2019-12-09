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
from cryptobi.db.dao.CBDAO import CBDAO
import argparse

parser = argparse.ArgumentParser(description = "Export infonodes about addresses, TX's and blocks.")
parser.add_argument('bitcoin_address', type=str, nargs=1)
args = parser.parse_args()

dao_a = CBDAO()
dao = dao_a.get_DAO()

nodes = dao.list_info_nodes()

for node in nodes:
    print("{}:{}:{}:{}".format(node.block_hash,node.tx_hash,node.address,node.content))