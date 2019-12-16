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

class CBBalance:
    """
    Simple struct for DB table cb_address_balance or equivalent
    """
    def __init__(self, cab_id, address, satoshis):
        self.cab_id = cab_id
        self.address = address
        self.satoshis = satoshis

