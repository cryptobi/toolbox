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

parser = argparse.ArgumentParser(description = "Query a Bitcoin block from the local databse.")
parser.add_argument('block_hash', type=str, nargs=1)
args = parser.parse_args()

dao_a = CBDAO()
dao = dao_a.get_DAO()

block_hash = args.block_hash
print(block_hash)

