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

class CBUtil:

    @staticmethod
    def safe_hash(phash, bytesize=32):
        """
        Null hashes returned from MySQL sometimes get
        coalesced to str. Ensure hash is always of bytes type.
        """

        tp = type(phash)

        if tp == bytearray or tp == bytes:
            return phash

        if phash is None:
            return bytes.fromhex("00" * bytesize)

        ret = phash
        if tp == str:
            if len(phash) > 0:
                if phash == ("\x00" * bytesize):
                    ret = bytes.fromhex("00" * bytesize)
                else:
                    ret = bytes.fromhex(phash)
            else:
                ret = bytes.fromhex("00" * bytesize)

        return ret