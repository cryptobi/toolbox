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

import cryptobi.toolbox.system.CBConfig
from cryptobi.db.dao.CBDAO import CBDAO
from cryptobi.model.blockchains.CBBlock import CBBlock
import sys

BATCH_SIZE = 5000
GENESIS_HASH = bytes.fromhex("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f")

dao_a = CBDAO()
dao = dao_a.get_DAO()

# fetch genesis block from DB
current_block = dao.get_block_by_hash(GENESIS_HASH)

if not current_block:
    print("Unable to load Genesis block from local DB. Have you built the DB using toolbox/db/blocks_inserter?")
    sys.exit(1)

# visit the entire blockchain, building the address graph
while not current_block is None:
    vtx = dao.list_tx_by_block(current_block.hash)
    for tx in vtx:
        inputs = dao.list_tx_in(tx.hash)
        outputs = dao.list_tx_out(tx.hash)
        pass