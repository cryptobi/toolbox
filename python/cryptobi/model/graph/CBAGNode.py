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

class CBAGNode:
    """
        A address graph node corresponding to the database table cb_address_graph
    """

    cag_id = 0
    n_vout = 0
    tx_from = ""
    n_vin = 0
    tx_to = ""
    address_from = ""
    address_to = ""
    satoshis = 0
    n_time = 0