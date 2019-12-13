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

class CBTx:

    """
        General transaction class.
    """
    def __init__(self):
        self.table_seq = 0
        self.hash_this_tx = False
        self.n_version = 1
        self.has_witness = False
        self.in_counter = 0
        self.lock_time = 0
        self.block_order = 0
        self.witness_hash = bytes.fromhex("00")
        self.hash_block = bytes.fromhex("00")

    def __repr__(self):
        return self.hash_this_tx.hex()



class CBTxIn:

    """
        Transaction input class.
    """

    def __init__(self):
        pass


class CBTxOut:

    """
        Transaction output class.
    """

    def __init__(self):
        pass


class CBTxOutAddress:

    """
        Utility class for database table cb_tx_out_addresses
    """

    table_seq = 0
    n_vout = 0
    hash_tx = 0
    address = ""
    script_req_sigs = 0
    script_type = 0

    def __init__(self):
        pass