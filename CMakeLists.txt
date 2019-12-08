#[[

 * Crypto.BI Toolbox
 * https://Crypto.BI/
 *
 * Author: José Fonseca (https://zefonseca.com/)
 *
 * Distributed under the MIT software license, see the accompanying
 * file COPYING or http://www.opensource.org/licenses/mit-license.php.
 *
 * cmake -D variables:
 * BITCOIN_SRC_DIR: Where Bitcoin Core was built.
 * BOOST_SRC_DIR: Where Boost Libraries were built.
 *
 *
 * Global symbols:
 * 		ext_libs : External libs, list of lib1, lib2 as in -llib1 -lib2
 *		ext_lib_paths : External lib paths, library directories.
 *		include_dirs : Required include directories.
 *
 * The build strategy consists of building subdirectory libraries recursively,
 * which are all linked into the final .a static library.
 * This final .a library is the Toolbox.
 * Each root subdirectory is linked to other subdirs recursively.
 * All other target executables are linked to the static lib.
 *

]]

if (BITCOIN_SRC_DIR)
	message(STATUS "Using BITCOIN_SRC_DIR = ${BITCOIN_SRC_DIR}")
else()
	message(FATAL_ERROR "Argument -DBITCOIN_SRC_DIR is mandatory. Please inform Bitcoin Core source dir.")
endif()

if (BOOST_SRC_DIR)
	message(STATUS "Using BOOST_SRC_DIR = ${BOOST_SRC_DIR}")
else()
	message(FATAL_ERROR "Argument -DBOOST_SRC_DIR is mandatory. Please inform Boost Library directory.")
endif()

if (MYSQLCONN_DIR)
	message(STATUS "Using MYSQLCONN_DIR = ${MYSQLCONN_DIR}")
else()
	message(FATAL_ERROR "Argument -DMYSQLCONN_DIR is mandatory. Please inform MySQL Connector/C++ directory.")
endif()

if (SPDLOG_DIR)
	message(STATUS "Using SPDLOG_DIR = ${SPDLOG_DIR}")
else()
	message(FATAL_ERROR "Argument -DSPDLOG_DIR is mandatory. Please inform spdlog directory.")
endif()

message(STATUS "")
message(STATUS "**************************************************")
message(STATUS "* Starting Crypto.BI Toolbox Build/Configuration *")
message(STATUS "**************************************************")
message(STATUS "")

cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(CryptoBI-Toolbox LANGUAGES CXX VERSION 0.0.1)

message(STATUS "Using PROJECT_SOURCE_DIR = ${PROJECT_SOURCE_DIR}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g3")

# EXAMPLES SECTION ---------------------------------------------------

if (CB_BUILD_EXAMPLES)
	add_subdirectory(examples/)
else()
	add_subdirectory(examples/ EXCLUDE_FROM_ALL)
endif()




set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_BUILD_TYPE RelWithDebInfo)



# TEST SECTION --------------------------------------------------------

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)
endif()

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
    add_subdirectory(tests)
endif()

# MAIN SECTION --------------------------------------------------------

# Several libraries provided by Bitcoin Core.
# Others are generally available.
set(ext_libs bitcoin_server
bitcoinconsensus
bitcoin_wallet_tool
bitcoin_wallet
bitcoinqt
memenv
bitcoin_cli
secp256k1
bitcoin_crypto_base
bitcoin_crypto_shani
bitcoin_crypto_sse41
bitcoin_crypto_avx2
bitcoin_consensus
bitcoin_common
bitcoin_util
event_pthreads
crypto
event
leveldb
leveldb_sse42
pthread
db
db_cxx
boost_chrono
boost_filesystem
boost_system
boost_thread
univalue
boost_program_options
crypto
ssl
spdlog
pthread
mysqlcppconn-static
mysqlclient
dl
z
)

set(ext_lib_paths ${BITCOIN_SRC_DIR}/src/.libs
${BITCOIN_SRC_DIR}/src/leveldb
${BOOST_SRC_DIR}/stage/lib
${BITCOIN_SRC_DIR}/src/crypto
${BITCOIN_SRC_DIR}/src/secp256k1/.libs/
${BITCOIN_SRC_DIR}/src/univalue/.libs/
${BITCOIN_SRC_DIR}/src/qt/
${BITCOIN_SRC_DIR}/src/
${MYSQLCONN_DIR}/jdbc/
${MYSQLCONN_DIR}/
${SPDLOG_DIR}/build/
)

set(include_dirs $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>
${BOOST_SRC_DIR}
${BITCOIN_SRC_DIR}/src/
${BITCOIN_SRC_DIR}/src/leveldb/include
${BITCOIN_SRC_DIR}/src/univalue/include
${MYSQLCONN_DIR}/include/
${SPDLOG_DIR}/include/
${SPDLOG_DIR}/include/spdlog/
)

# COMPONENT SUBDIRECTORIES

add_subdirectory(toolbox/)
add_subdirectory(model/)
add_subdirectory(db/)

# BUILD TOOLBOX LIBRARY

add_library(cbtoolbox-static STATIC CB.cpp CB.h)
target_include_directories(cbtoolbox-static PUBLIC ${include_dirs})
target_link_directories(cbtoolbox-static PUBLIC ${ext_lib_paths})
target_link_libraries(cbtoolbox-static PUBLIC ${ext_libs})
target_link_libraries(cbtoolbox-static PUBLIC db-dao-mysql)
target_link_libraries(cbtoolbox-static PUBLIC db-dao)
target_link_libraries(cbtoolbox-static PUBLIC db-fs-bitcoin)
target_link_libraries(cbtoolbox-static PUBLIC db-fs)
target_link_libraries(cbtoolbox-static PUBLIC db)
target_link_libraries(cbtoolbox-static PUBLIC toolbox-system)
target_link_libraries(cbtoolbox-static PUBLIC toolbox)
target_link_libraries(cbtoolbox-static PUBLIC model-blockchains)
target_link_libraries(cbtoolbox-static PUBLIC model
)
