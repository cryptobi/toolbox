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

from cryptobi.crypto.CBUtil import CBUtil

class CBTx:

    """
        General transaction class.
    """
    def __init__(self):
        self.table_seq = 0
        self.hash_this_tx = bytes.fromhex("00")
        self.n_version = 1
        self.has_witness = False
        self.in_counter = 0
        self.lock_time = 0
        self.block_order = 0
        self.witness_hash = bytes.fromhex("00")
        self.hash_block = bytes.fromhex("00")

    def __repr__(self):
        rd = self.__dict__
        ret = ""
        try:
            rd["hash_this_tx"] = CBUtil.safe_hash(rd["hash_this_tx"]).hex()
            rd["hash_block"] = CBUtil.safe_hash(rd["hash_block"]).hex()
            rd["witness_hash"] = CBUtil.safe_hash(rd["witness_hash"]).hex()
            ret = str(rd)
        except:
            pass

        return ret



class CBTxIn:

    """
        Transaction input class.
    """

    def __init__(self, table_seq, txid, n_vout, hash_this_tx, n_sequence):
        self.table_seq = table_seq
        self.txid = txid
        self.n_vout = n_vout
        self.hash_this_tx = hash_this_tx
        self.n_sequence = n_sequence

    def __repr__(self):
        rd = self.__dict__
        ret = ""
        try:
            rd["hash_this_tx"] = CBUtil.safe_hash(rd["hash_this_tx"]).hex()
            rd["txid"] = CBUtil.safe_hash(rd["txid"]).hex()
            ret = str(rd)
        except Exception as e:
            print("Error in TxIn")
            print(e)
            pass

        return ret

class CBTxOut:

    """
        Transaction output class.
    """

    def __init__(self, table_seq, satoshis, n_vout, hash_this_tx, hash_tx_spent, script_req_sigs, script_type):
        self.table_seq = table_seq
        self.satoshis = satoshis
        self.n_vout = n_vout
        self.hash_this_tx = hash_this_tx
        self.hash_tx_spent = hash_tx_spent
        self.script_req_sigs = script_req_sigs
        self.script_type = script_type

    def __repr__(self):
        rd = self.__dict__
        ret = ""
        try:
            rd["hash_this_tx"] = CBUtil.safe_hash(rd["hash_this_tx"]).hex()
            rd["hash_tx_spent"] = CBUtil.safe_hash(rd["hash_tx_spent"]).hex()
            ret = str(rd)
        except Exception as e:
            print("Error in Txout")
            print(e)

        return ret

class CBTxOutAddress:

    """
        Utility class for database table cb_tx_out_addresses
    """

    def __init__(self, table_seq, n_vout, hash_tx, address, script_req_sigs, script_type):
        self.table_seq = table_seq
        self.n_vout = n_vout
        self.hash_tx = hash_tx
        self.address = address
        self.script_req_sigs = script_req_sigs
        self.script_type = script_type

    def __repr__(self):
        rd = self.__dict__
        ret = ""
        try:
            rd["hash_tx"] = CBUtil.safe_hash(rd["hash_tx"]).hex()
            ret = str(rd)
        except:
            pass

        return ret