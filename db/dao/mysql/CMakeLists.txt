#[[

 * Crypto.BI Toolbox
 * https://Crypto.BI/
 *
 * Author: José Fonseca (https://zefonseca.com/)
 *
 * Distributed under the MIT software license, see the accompanying
 * file COPYING or http://www.opensource.org/licenses/mit-license.php.

]]

add_library(db-dao-mysql OBJECT CBMySQL.cpp CBMySQL.h)
target_include_directories(db-dao-mysql PUBLIC ${include_dirs})