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
#ifndef TOOLBOX_SYSTEM_CBCONFIG_H_
#define TOOLBOX_SYSTEM_CBCONFIG_H_

#include <memory>
#include <string>
#include <map>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <fstream>
#include "inipp.h"
#include <optional>
#include <cstdlib>

// use the bundled fmt in spdlog
#include <fmt/bundled/format.h>

/*
 * Logging is set up here in the config section so we don't get
 * cyclic includes up higher in the hierarchy.
 * Moving logging into an application object at the helm, such as
 * /CB.h would require CB to include DBConfig and DBConfig would
 * have to include CB in order to be able to log messages.
 *
 * */
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>

namespace toolbox::system {

namespace po = boost::program_options;

class CBConfig {

	const std::string DEFAULT_BITCOIN_DATADIR = ".bitcoin";
	const std::string DEFAULT_USER_DIR = ".Crypto.BI";
	const std::string DEFAULT_CONFIG_FILENAME = "crypto.bi.ini";
	const std::string DEFAULT_LOG_FILE = "crypto.bi.log";
	const std::string DEFAULT_LOG_ERROR_FILE = "crypto.bi.err";
	const int LOG_FILE_SIZE = 1048576 * 5;
	const int LOG_FILE_DAYS = 3;

	std::map<std::string, std::string> config_map;
	// config_file -> boost::fs::path to configuration file
	// bitcoin_blocks_dir bitcoin core /blocks data directory path

	// singleton systemwide config instance
	static std::shared_ptr<CBConfig> _config;

	// keep the options clutter out

	std::map<std::string, std::string> ___cb_opts{
		// this is also included from python
		#include "config_options.map"
	};



    po::options_description desc{"Allowed Options"};
	po::variables_map vm;

	std::shared_ptr<spdlog::logger> console;
	std::shared_ptr<spdlog::logger> err_logger;
	std::shared_ptr<spdlog::logger> file_logger;
	std::shared_ptr<spdlog::logger> file_loggerr;



public:

	CBConfig();
	virtual ~CBConfig();
	// gets singleton systemwide config instance
	static std::shared_ptr<CBConfig> get_config();
	// user's home directory
	boost::filesystem::path get_user_dir();
	// reads configuration from file
	bool read_from_file(const boost::filesystem::path &config_path);
	// verifies and creates dir if nonexistent
	bool init_dir(const boost::filesystem::path &config_dir);
	// get a configuration variable
	std::optional<std::string> get_conf(const std::string &key);
	// start the system and apply the right config order
	bool init_config(int argc, char **argv);
	// apply defaults before reading config file and command line
	bool apply_default_config();
	bool process_command_line(int argc, char **argv, bool apply);

	bool log_message(const std::string &message);
	bool log_error(const std::string &error);

	const std::map<std::string, std::string>& get_config_map() const {
		return config_map;
	}

	void set_config_map(const std::map<std::string, std::string> &configMap) {
		config_map = configMap;
	}

};

} // namespace
#endif /* TOOLBOX_SYSTEM_CBCONFIG_H_ */
