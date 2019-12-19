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
from cryptobi.model.blockchains.CBAddress import CBAddress
from cryptobi.model.blockchains.CBTx import CBTxOutAddress
from cryptobi.model.graph.CBAGNode import CBAGNode
from cryptobi.crypto.CBUtil import CBUtil
from cryptobi.db.dao.CBDAO import CBDAO

config = CBConfig.get_config()
dao = CBDAO.get_DAO()
current_block = CBBlock.genesis()
height = 0


def map_input_addresses(inputs):
    """
    Map inputs to origin TX outputs.
    """
    ret = []
    for input in inputs:
        if CBUtil.safe_hash(input.txid) == bytes.fromhex("00" * 32):
            ix = CBTxOutAddress(input.table_seq, input.n_vout, CBUtil.safe_hash(input.txid), CBAddress.COINBASE_ADDRESS, -1, -1)
            ret.append(ix)
        else:
            ix = dao.get_tx_out_address_byhash_nvout(CBUtil.safe_hash(input.txid), input.n_vout)
            if ix:
                ret.append(ix)
            else:
                print("ERROR OBTAINING OUTPUT ADDRESS FOR {}/{}".format(CBUtil.safe_hash(input.txid).hex(), input.n_vout))

    return ret



while current_block:

    print("BLOCK {}".format(CBUtil.safe_hash(current_block.hash).hex()))
    txs = dao.list_tx_by_block(CBUtil.safe_hash(current_block.hash))

    for tx in txs:

        print("TX {}".format(CBUtil.safe_hash(tx.hash_this_tx).hex()))

        inputs = dao.list_tx_in(CBUtil.safe_hash(tx.hash_this_tx))
        prev_outputs = map_input_addresses(inputs)
        outputs = dao.list_tx_out(CBUtil.safe_hash(tx.hash_this_tx))
        output_addresses = dao.list_tx_out_addresses(CBUtil.safe_hash(tx.hash_this_tx))

        for prev_out in prev_outputs:
            for output in output_addresses:
                txout = outputs[output.n_vout]
                cbag = CBAGNode(0, prev_out.n_vout, prev_out.hash_tx, output.n_vout, output.hash_tx, prev_out.address, output.address, txout.satoshis, current_block.ntime)
                dao.insert_address_graph(cbag)
                print(cbag)

    print()

    current_block = dao.get_next_block(CBUtil.safe_hash(current_block.hash))
    height += 1

