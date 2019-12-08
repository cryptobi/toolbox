#[[

 * Crypto.BI Toolbox
 * https://Crypto.BI/
 *
 * Author: José Fonseca (https://zefonseca.com/)
 *
 * Distributed under the MIT software license, see the accompanying
 * file COPYING or http://www.opensource.org/licenses/mit-license.php.

]]

add_library(model-graph OBJECT CBInfoNode.cpp CBInfoNode.h)
target_include_directories(model-graph PUBLIC ${include_dirs})