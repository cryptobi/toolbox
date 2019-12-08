#[[

 * Crypto.BI Toolbox
 * https://Crypto.BI/
 *
 * Author: José Fonseca (https://zefonseca.com/)
 *
 * Distributed under the MIT software license, see the accompanying
 * file COPYING or http://www.opensource.org/licenses/mit-license.php.

]]

add_subdirectory(dao/)
add_subdirectory(fs/)

add_library(db OBJECT CBDB.h CBDB.cpp)
target_link_libraries(db PUBLIC db-fs)
target_link_libraries(db PUBLIC db-dao)
