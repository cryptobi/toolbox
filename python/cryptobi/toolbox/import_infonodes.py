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
import os.path
import sys
from cryptobi.db.dao.CBDAO import CBDAO
import argparse

parser = argparse.ArgumentParser(description = "Import infonodes about addresses, TX's and blocks.")
parser.add_argument('filename', type=str, nargs=1)
args = parser.parse_args()

dao_a = CBDAO()
dao = dao_a.get_DAO()

filename = args.filename

if not os.path.exists(filename):
    print("File {} does not exist.".format(filename))
    sys.exit(1)

content = ""
with open(filename, "r") as fil:
    content = fil.read()

