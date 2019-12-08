#[[

 * Crypto.BI Toolbox
 * https://Crypto.BI/
 *
 * Author: José Fonseca (https://zefonseca.com/)
 *
 * Distributed under the MIT software license, see the accompanying
 * file COPYING or http://www.opensource.org/licenses/mit-license.php.

]]

add_subdirectory(system/)
add_subdirectory(db/)

add_library(toolbox OBJECT CBToolbox.h CBToolbox.cpp)
target_link_libraries(toolbox PUBLIC toolbox-system)