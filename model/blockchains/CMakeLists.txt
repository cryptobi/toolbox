#[[

 * Crypto.BI Toolbox
 * https://Crypto.BI/
 *
 * Author: José Fonseca (https://zefonseca.com/)
 *
 * Distributed under the MIT software license, see the accompanying
 * file COPYING or http://www.opensource.org/licenses/mit-license.php.

]]

add_library(model-blockchains OBJECT
CBBlock.cpp CBBlock.h
CBHash.cpp CBHash.h
CBTx.cpp CBTx.h)

target_include_directories(model-blockchains PUBLIC ${include_dirs})