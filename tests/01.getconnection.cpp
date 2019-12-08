/*
 *
 * Crypto.BI Toolbox
 * https://Crypto.BI/
 *
 * Author: José Fonseca (https://zefonseca.com/)
 *
 * Distributed under the MIT software license, see the accompanying
 * file COPYING or http://www.opensource.org/licenses/mit-license.php.
 *
 */

#include <cstdlib>
#include <boost/filesystem.hpp>
#include <iostream>
#include "db/dao/mysql/CBMySQL.h"

#define NUMBER_LOOPS 100000000

int main(int argc, char **argv) {

	auto config = toolbox::system::CBConfig::get_config();
	config->init_config(argc, argv);

	db::dao::mysql::CBMySQL m;

	for (int i=0;i<NUMBER_LOOPS;i++) {
		auto x = m.get_connection();
		if (x->conn == nullptr) {
			return EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;
}
