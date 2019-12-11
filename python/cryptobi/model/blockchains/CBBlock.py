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


 A Satoshi Address: 1A1zP1eP5QGefi2DMPTfTL5SLmv7DivfNa

"""

from cryptobi.model.blockchains.CBTx import CBTx

class CBBlock:

    def __init__(self):
        self.table_seq = 1
        self.height = 0
        self.vtx = []
        self.filename = "blk00000.dat"
        self.file_byte_offset = 0
        self.n_version = 1
        self.hash = "000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"
        self.merkle_root = "4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"
        self.hash_prev_block = "0000000000000000000000000000000000000000000000000000000000000000"
        self.hash_next_block = "0000000000000000000000000000000000000000000000000000000000000000"
        self.ntime = 1231006505
        self.bits = 486604799
        self.nonce = 2083236893


    @staticmethod
    def genesis():
        """
        Statically build genesis block.
        The default CBBlock constructor returns the genesis block.
        """
        return CBBlock()

