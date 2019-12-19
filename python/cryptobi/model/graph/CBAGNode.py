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


class CBAGNode:
    """
        A address graph node corresponding to the database table cb_address_graph
    """

    def __init__(self, cag_id, n_vout, tx_from, n_vin, tx_to, address_from, address_to, satoshis, n_time):
        self.cag_id = cag_id
        self.n_vout = n_vout
        self.tx_from = CBUtil.safe_hash(tx_from)
        self.n_vin = n_vin
        self.tx_to = CBUtil.safe_hash(tx_to)
        self.address_from = address_from
        self.address_to = address_to
        self.satoshis = satoshis
        self.n_time = n_time

    def __repr__(self):
        rd = self.__dict__
        ret = ""
        try:
            rd["tx_from"] = CBUtil.safe_hash(rd["tx_from"]).hex()
            rd["tx_to"] = CBUtil.safe_hash(rd["tx_to"]).hex()
            ret = str(rd)
        except:
            pass

        return ret