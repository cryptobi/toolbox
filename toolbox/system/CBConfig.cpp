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
#include "CBConfig.h"

namespace toolbox::system {

	std::shared_ptr<CBConfig> CBConfig::_config;

	CBConfig::CBConfig() {

		// initialize loggers
		console = spdlog::stdout_color_mt("console");
		err_logger = spdlog::stderr_color_mt("stderr");
		file_logger = spdlog::rotating_logger_mt("file_logger", DEFAULT_LOG_FILE , LOG_FILE_SIZE, LOG_FILE_DAYS);
		file_loggerr = spdlog::rotating_logger_mt("file_error_logger", DEFAULT_LOG_ERROR_FILE , LOG_FILE_SIZE, LOG_FILE_DAYS);

		// make sure user config dir exists
		boost::filesystem::path idir = get_user_dir() / boost::filesystem::path{DEFAULT_USER_DIR};
		bool id_ret = init_dir(idir);

		if (!id_ret) {
			log_error("init_dir() unable to initialize config directory.");
		}

		auto obx = desc.add_options();
		for (const auto &[k,v] : ___cb_opts) {
			obx(k.c_str(), po::value<std::string>(), v.c_str());
		}
	}

	CBConfig::~CBConfig() {

	}

	std::shared_ptr<CBConfig> CBConfig::get_config() {
		if (_config == nullptr) {
			_config = std::make_shared<CBConfig>();
		}
		return _config;
	}

	bool CBConfig::read_from_file(const boost::filesystem::path &config_path) {
		inipp::Ini<char> ini;
		std::ifstream is(config_path.generic_string());
		ini.parse(is);
		ini.interpolate();
		auto sym = ini.get_symbols();
		for (const auto &[name, value] : sym) {
			config_map[name.substr(3, name.length()-4)] = value;
		}
		return true;
	}

	bool CBConfig::init_dir(const boost::filesystem::path &config_dir) {

		if (boost::filesystem::exists(config_dir)) {
			return true;
		}

		return boost::filesystem::create_directory(config_dir);
	}

	std::optional<std::string> CBConfig::get_conf(const std::string &key) {
		try {
			auto ix = config_map.at(key);
			return ix;
		} catch (std::out_of_range &soor) {
			return {};
		}
	}

	/** Apply the correct order of parameter initialization.
	 *
	 * First, apply system defaults.
	 * Then, options file.
	 * Lastly the command line.
	 * Each step overwrites the previous, so the priority is applied
	 * in reverse, with the command line overriding the others.
	 *
	 * This is the function called by Toolbox programs' main().
	 *
	 * */
	bool CBConfig::init_config(int argc, char **argv) {
		auto _c = get_config(); // get or create config singleton
		apply_default_config(); // step 1
		// read options, see if config file changed
		// false indicates do not apply to config
		process_command_line(argc, argv, false);

		if (vm.count("conf") > 0) {
			_c->config_map["conf"] = vm["conf"].as<std::string>();
		}

		boost::filesystem::path cfile{_c->config_map["conf"]};

		if (boost::filesystem::exists(cfile)) {
			read_from_file(cfile); // step 2
		} else {
			log_error("Configuration file not found.");
		}

		// command line redone again, last step so it
		// overwrites all other config options
		process_command_line(argc, argv, true); // step 3

		// we're still here, noexcept. so, true
		return true;
	}

	boost::filesystem::path CBConfig::get_user_dir() {
		const char *homeDir = getenv("HOME");
		std::string hdir{homeDir};
		return boost::filesystem::path{hdir};
	}

	bool CBConfig::apply_default_config() {
		auto _c = get_config(); // get or create config singleton

		// set default blocks dir
		_c->config_map["bitcoin_blocks_dir"] = (
				get_user_dir() /
				boost::filesystem::path{DEFAULT_BITCOIN_DATADIR} /
				boost::filesystem::path{"blocks"}
				).generic_string();

		// set default config file
		_c->config_map["conf"] = (
				get_user_dir() /
				boost::filesystem::path{DEFAULT_USER_DIR} /
				boost::filesystem::path{DEFAULT_CONFIG_FILENAME}
				).generic_string();

		// set default log dir
		_c->config_map["log_dir"] = (
				get_user_dir() /
				boost::filesystem::path{DEFAULT_USER_DIR}
				).generic_string();

		return true;
	}

	/** Processes command line options.
	 * Only applies to overall config_map if apply is true.
	 *
	 * */
	bool CBConfig::process_command_line(int argc, char **argv, bool apply) {

		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);

		// copy cmd line opts to internal config
		if (apply) {
			for (const auto &v : vm) {
				config_map[v.first] = v.second.as<std::string>();
			}
		}

		return true;
	}

	bool CBConfig::log_message(const std::string &message) {
	    spdlog::get("console")->info(message);
	    if (get_conf("log_to_file") == "1") {
	    	spdlog::get("file_logger")->info(message);
	    }
	    return true;
	}

	bool CBConfig::log_error(const std::string &error) {
		spdlog::get("stderr")->info(error);
	    if (get_conf("log_to_file") == "1") {
	    	spdlog::get("file_error_logger")->info(error);
	    }
		return true;
	}

} // namespace


