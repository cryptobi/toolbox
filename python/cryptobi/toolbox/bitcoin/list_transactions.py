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

config = CBConfig.get_config()
dao = CBDAO.get_DAO()
current_block = CBBlock.genesis()
height = 0

# walk the blockchain, listing TX hashes
while current_block:

    print("BLOCK {}".format(current_block.hash.hex()))
    txs = dao.list_tx_by_block(current_block.hash)

    for tx in txs:
        print("TX {}".format(tx.hash_this_tx.hex()))

    print()

    current_block = dao.get_next_block(current_block.hash)
    height += 1

