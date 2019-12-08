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
#include "toolbox/system/CBConfig.h"
#include "db/dao/CBDAO.h"
#include <fmt/bundled/format.h>

int main(int argc, char **argv) {

	auto config = toolbox::system::CBConfig::get_config();
	config->init_config(argc, argv);

	config->log_message("Testing Database Server Connectivity....");

	auto dao = db::dao::CBDAO::get_DAO();
	auto res = dao->test_connection();

	if (res == "ERROR") {
		config->log_message("Unable to query the configured database server. Please check your Toolbox configuration and try again.");
		return EXIT_FAILURE;
	}

	config->log_message("SUCCESS! Your database server is configured correctly.");
	config->log_message(fmt::format("Running server version: {}", res));
	config->log_message("You may now run all database dependent Crypto.BI Toolbox programs.");

	return EXIT_SUCCESS;
}
