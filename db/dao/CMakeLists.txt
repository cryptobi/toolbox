#[[

 * Crypto.BI Toolbox
 * https://Crypto.BI/
 *
 * Author: José Fonseca (https://zefonseca.com/)
 *
 * Distributed under the MIT software license, see the accompanying
 * file COPYING or http://www.opensource.org/licenses/mit-license.php.

]]

add_subdirectory(mysql/)
add_library(db-dao OBJECT CBDAO.cpp CBDAO.h CBDAODriver.cpp CBDAODriver.h)
target_include_directories(db-dao PUBLIC ${include_dirs})
target_link_libraries(db-dao PUBLIC db-dao-mysql)