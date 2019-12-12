"""

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

"""

from datetime import datetime
from pathlib import Path
import sys
import os
import configparser
import argparse

class CBConfig:

    """
        Crypto.BI Toolbox global configuration object.
        Use static method CBConfig.get_conf() to obtain a singleton.
    """

    # static consts
    DEFAULT_BITCOIN_DATADIR = ".bitcoin"
    DEFAULT_USER_DIR = ".Crypto.BI"
    DEFAULT_CONFIG_FILENAME = "crypto.bi.ini"
    DEFAULT_LOG_FILE = "crypto.bi.log"
    DEFAULT_LOG_ERROR_FILE = "crypto.bi.err"
    LOG_FILE_SIZE = 1048576 * 5
    LOG_FILE_DAYS = 3
    ENVIRONMENT_VAR_HOME = "CRYPTOBI_HOME"

    static_conf = None

    def __init__(self):
        self.cmd_parser = None
        self.config_map = {}
        self.init_config()

    @staticmethod
    def get_config():
        if CBConfig.static_conf == None:
            CBConfig.static_conf = CBConfig()
        return CBConfig.static_conf

    @staticmethod
    def get_user_dir():
        return str(Path.home())

    def read_from_file(self, config_path):
        cf = configparser.ConfigParser()
        cf.read(config_path)
        self.config_map.update(cf['DEFAULT'])

    def init_dir(self, config_dir) :
        if not os.path.isdir(config_dir):
            os.mkdir(config_dir)

    def get_conf(self, key):
        if key in self.config_map.keys():
            return self.config_map[key]
        return

    def apply_default_config(self):
        self.config_map["bitcoin_blocks_dir"] = os.path.join(CBConfig.get_user_dir(), CBConfig.DEFAULT_BITCOIN_DATADIR, "blocks")
        self.config_map["conf"] = os.path.join(CBConfig.get_user_dir(), CBConfig.DEFAULT_USER_DIR, CBConfig.DEFAULT_CONFIG_FILENAME)
        self.config_map["log_dir"] = os.path.join(CBConfig.get_user_dir(), CBConfig.DEFAULT_USER_DIR)

    def process_command_line(self, apply=True):
        parser = argparse.ArgumentParser()
        cryptobi_home = os.getenv(CBConfig.ENVIRONMENT_VAR_HOME)

        if cryptobi_home == None:
            self.log_error("Environment variable {} must be set in order to use Python Crypto.BI Toolbox".format(CBConfig.ENVIRONMENT_VAR_HOME))
            sys.exit(1)

        tsp = os.path.join(cryptobi_home,"toolbox","system","config_options.map")

        with open(tsp, 'r') as f:
            cft = f.read()
        params = "[" + cft + "]"
        param_list = eval(params.replace("\",","\":"))

        for ple in param_list:
            for k in ple.keys():
                v = ple[k]
                ak = "--" + k
                parser.add_argument(ak,help=v,type=str)

        parser.add_argument('listargs', type=str, nargs='?')
        args = vars(parser.parse_args())

        if (apply):
            for k in args:
                v = args[k]
                if (v is not None):
                    self.config_map[k] = v

    def log_message(self, message):
        print("[INFO] {} : {}".format(datetime.now(), message))

    def log_error(self, error):
        print("[ERROR] {} : {}".format(datetime.now(), error), file=sys.stderr)

    def init_config(self):
        self.apply_default_config()
        self.process_command_line(True)
        self.read_from_file(self.config_map["conf"])
        self.process_command_line(True)