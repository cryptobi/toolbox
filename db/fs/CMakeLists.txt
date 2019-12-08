#[[

 * Crypto.BI Toolbox
 * https://Crypto.BI/
 *
 * Author: José Fonseca (https://zefonseca.com/)
 *
 * Distributed under the MIT software license, see the accompanying
 * file COPYING or http://www.opensource.org/licenses/mit-license.php.

]]

add_subdirectory(bitcoin/)

add_library(db-fs OBJECT
CBBlockCallback.h CBBlockCallback.cpp
CBBlockchainReader.h
CBBlockEntry.h CBBlockEntry.cpp
CBFS.h CBFS.cpp
)

target_include_directories(db-fs PUBLIC ${include_dirs})
target_link_libraries(db-fs PUBLIC db-fs-bitcoin)