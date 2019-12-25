#[[

 * Crypto.BI Toolbox
 * https://Crypto.BI/
 *
 * Author: José Fonseca (https://zefonseca.com/)
 *
 * Distributed under the MIT software license, see the accompanying
 * file COPYING or http://www.opensource.org/licenses/mit-license.php.

]]

macro(toolbox_prog fn)
	add_executable(${fn} "${fn}.cpp")
	target_include_directories(${fn} PUBLIC ${include_dirs})
	target_link_libraries(${fn} cbtoolbox-static)
endmacro()

toolbox_prog("blocks_inserter")
toolbox_prog("blocks_to_sql")
toolbox_prog("test_connection")
toolbox_prog("blocks_updater")