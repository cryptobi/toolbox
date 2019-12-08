#[[

 * Crypto.BI Toolbox
 * https://Crypto.BI/
 *
 * Author: José Fonseca (https://zefonseca.com/)
 *
 * Distributed under the MIT software license, see the accompanying
 * file COPYING or http://www.opensource.org/licenses/mit-license.php.

]]

macro(example_case fn)
	add_executable(${fn} "${fn}.cpp")
	target_include_directories(${fn} PUBLIC ${include_dirs})
	target_link_libraries(${fn} cbtoolbox-static)
endmacro()


example_case("fs_block_reader")
example_case("fs_list_blockfiles")
