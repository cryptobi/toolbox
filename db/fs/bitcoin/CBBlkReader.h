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
#ifndef DB_FS_BITCOIN_CBBLKREADER_H_
#define DB_FS_BITCOIN_CBBLKREADER_H_

#include <config/bitcoin-config.h>
#include <vector>
#include <cstdio>
#include <string>
#include <exception>
#include <stdexcept>
#include <streams.h>
#include <cstdio>
#include <vector>
#include <utility>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <clientversion.h>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/lexical_cast.hpp>
#include <consensus/consensus.h>
#include "db/fs/bitcoin/CBBlkEntry.h"
#include "db/fs/CBBlockCallback.h"
#include "model/blockchains/CBBlock.h"
#include <fmt/bundled/format.h>

namespace db::fs::bitcoin {

class CBBlkReader {

public:
	static FILE *blk_open_autofile(const boost::filesystem::path &file_path);
	static std::vector<CBBlkEntry> read_blocks(const boost::filesystem::path &file_path);
	static void read_blocks_callback(const boost::filesystem::path &file_path, db::fs::CBBlockCallback &callback, uint64_t byte_offset_start=0);
	// Reads file_path directory and finds the last block file number
	// E.g. blk01101.dat returns 1101
	static int get_last_blockfile_nr(const boost::filesystem::path &file_path);
	// Generates a list of full path filenames
	static std::vector<boost::filesystem::path> generate_block_filenames(const boost::filesystem::path &file_path, const int lastnr);
	// util method to refactor number extraction
	// if for some reason blk file names change, change here.
	static inline std::vector<boost::filesystem::path> list_block_filenames(const boost::filesystem::path &file_path) {
		return generate_block_filenames(file_path, get_last_blockfile_nr(file_path));
	}
	static inline int ___extract_nr(const std::string &str) {
		std::string snr = str.substr(3,5);
		return std::stoi(snr);
	}
};



} /* namespace fs */

#endif /* DB_FS_BITCOIN_CBBLKREADER_H_ */
