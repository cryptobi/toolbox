#[[

 * Crypto.BI Toolbox
 * https://Crypto.BI/
 *
 * Author: José Fonseca (https://zefonseca.com/)
 *
 * Distributed under the MIT software license, see the accompanying
 * file COPYING or http://www.opensource.org/licenses/mit-license.php.

]]

macro(CB_test_case cb_test_name cb_src_file cb_target)
	add_executable(${cb_target} ${cb_src_file})
	target_link_libraries(${cb_target} cbtoolbox-static)
	target_include_directories(${cb_target} PUBLIC ${include_dirs})
	add_test(NAME ${cb_test_name} COMMAND ${cb_target})
endmacro()

CB_test_case("00includes" "00.includes.cpp" "00includes_t")
CB_test_case("01getconnection" "01.getconnection.cpp" "01getconnection_t")