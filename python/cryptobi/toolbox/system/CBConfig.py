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
from operator import itemgetter
from pathlib import Path
import sys
import os
import configparser
import argparse
import argparse

class CBConfig:

    DEFAULT_BITCOIN_DATADIR = ".bitcoin"
    DEFAULT_USER_DIR = ".Crypto.BI"
    DEFAULT_CONFIG_FILENAME = "crypto.bi.ini"
    DEFAULT_LOG_FILE = "crypto.bi.log"
    DEFAULT_LOG_ERROR_FILE = "crypto.bi.err"
    LOG_FILE_SIZE = 1048576 * 5
    LOG_FILE_DAYS = 3

    config_map = {}

    @staticmethod
    def get_user_dir():
        return str(Path.home())

    @staticmethod
    def read_from_file(config_path):
        cf = configparser.ConfigParser()
        cf.read(config_path)
        CBConfig.config_map.update(cf['DEFAULT'])

    @staticmethod
    def init_dir(config_dir) :
        if not os.path.isdir(config_dir):
            os.mkdir(config_dir)

    @staticmethod
    def get_conf(key):
        if key in keys(CBConfig.config_map):
            return CBConfig.config_map[key]
        return

    @staticmethod
    def apply_default_config():
        CBConfig.config_map["bitcoin_blocks_dir"] = os.path.join(CBConfig.get_user_dir(), CBConfig.DEFAULT_BITCOIN_DATADIR, "blocks")
        CBConfig.config_map["conf"] = os.path.join(CBConfig.get_user_dir(), CBConfig.DEFAULT_USER_DIR, CBConfig.DEFAULT_CONFIG_FILENAME)
        CBConfig.config_map["log_dir"] = os.path.join(CBConfig.get_user_dir(), CBConfig.DEFAULT_USER_DIR)

    @staticmethod
    def process_command_line(apply=True):
        parser = argparse.ArgumentParser()
        tsp = os.path.join(os.path.dirname(__file__),"..","..","..","toolbox","system","config_options.map")

        with open(tsp, 'r') as f:
            cft = f.read()
        params = "[" + cft + "]"
        param_list = eval(params.replace("\",","\":"))

        for ple in param_list:
            for k in ple.keys():
                v = ple[k]
                ak = "--" + k
                parser.add_argument(ak,help=v,type=str)

        args = vars(parser.parse_args())

        if (apply):
            for k in args:
                v = args[k]
                if (v is not None):
                    CBConfig.config_map[k] = v

    @staticmethod
    def log_message(message):
        print("[INFO] {} : {}".format(datetime.now(), message))

    @staticmethod
    def log_error(error):
        print("[ERROR] {} : {}".format(datetime.now(), message), file=sys.stderr)


    @staticmethod
    def init_config():
        CBConfig.apply_default_config()
        CBConfig.process_command_line(True)
        CBConfig.read_from_file(CBConfig.config_map["conf"])
        CBConfig.process_command_line(True)

if __name__ == '__main__':
    CBConfig.init_config()
    print(CBConfig.config_map)