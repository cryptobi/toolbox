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

# KNOWN BUG: must load the mysql module before all other libraries
# so they don't load an incompatible version of openssl which causes
# random segfaults with mysql procedures
# mysql module is not used here but must be loaded first on some systems
from mysql import connector

from cryptobi.toolbox.system.CBConfig import CBConfig
from cryptobi.model.blockchains.CBBlock import CBBlock
from cryptobi.db.dao.CBDAO import CBDAO
import sys

config = CBConfig.get_config()

dao_a = CBDAO()
dao = dao_a.get_DAO()

addr = config.get_conf("listargs")

if not len(addr) > 0:
    config.log_error("Invalid block hash.")
    sys.exit(1)

block_hash = bytes.fromhex(addr)

# genesis block
if block_hash == CBBlock.genesis().hash:
    print(CBBlock())
    sys.exit(1)

block = dao.get_block_by_hash(block_hash)

if block:
    vtx = dao.list_tx_by_block(block.hash)
    block.vtx = vtx

print(block)
