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
dao = CBDAO.get_DAO()

# bootstrap the blockchain
current_block = CBBlock.genesis()
height = 0

# walk the blockchain
while current_block:
    print("{:10} {}".format(height, current_block.hash.hex()))
    height += 1
    current_block = dao.get_next_block(current_block.hash)

