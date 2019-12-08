#[[

 * Crypto.BI Toolbox
 * https://Crypto.BI/
 *
 * Author: José Fonseca (https://zefonseca.com/)
 *
 * Distributed under the MIT software license, see the accompanying
 * file COPYING or http://www.opensource.org/licenses/mit-license.php.

]]

add_library(db-fs-bitcoin OBJECT
 CBBitcoinChainReader.h
 CBBlkEntry.h CBBlkEntry.cpp
 CBBlkReader.h CBBlkReader.cpp
 CBEncoder.h CBEncoder.cpp
 )

target_include_directories(db-fs-bitcoin PUBLIC ${include_dirs})