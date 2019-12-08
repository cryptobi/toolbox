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

parser = argparse.ArgumentParser(description = "Query a Bitcoin TX from the local databse.")
parser.add_argument('tx_hash', type=str, nargs=1)
args = parser.parse_args()

dao_a = CBDAO()
dao = dao_a.get_DAO()

tx_hash = args.bitcoin_address
print(tx_hash)

