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

class CBBlockFile:

    """
    Trivial helper class that represents an entry into table
    cb_blockchain_files or equivalent.
    """

    def __init__(self, table_seq, hash_this_block, filename, byte_offset):
        self.table_seq = table_seq
        self.hash_this_block = hash_this_block
        self.filename = filename
        self.byte_offset = byte_offset
