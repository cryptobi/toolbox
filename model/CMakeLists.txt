#[[

 * Crypto.BI Toolbox
 * https://Crypto.BI/
 *
 * Author: José Fonseca (https://zefonseca.com/)
 *
 * Distributed under the MIT software license, see the accompanying
 * file COPYING or http://www.opensource.org/licenses/mit-license.php.

]]

add_subdirectory(blockchains/)
add_subdirectory(graph/)

add_library(model OBJECT CBModel.h CBModel.cpp)
target_link_libraries(model PUBLIC model-graph)
target_link_libraries(model PUBLIC model-blockchains)