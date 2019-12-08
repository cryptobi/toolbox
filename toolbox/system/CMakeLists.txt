#[[

 * Crypto.BI Toolbox
 * https://Crypto.BI/
 *
 * Author: José Fonseca (https://zefonseca.com/)
 *
 * Distributed under the MIT software license, see the accompanying
 * file COPYING or http://www.opensource.org/licenses/mit-license.php.

]]

add_library(toolbox-system OBJECT CBConfig.cpp CBConfig.h inipp.h config_options.map)
target_include_directories(toolbox-system PUBLIC ${include_dirs})
