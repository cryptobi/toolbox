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

// Reads Bitcoin blocks from local directory data files.
// Shows usage of db::fs::CBFS functions

#include <cstdlib>
#include <boost/filesystem.hpp>
#include <iostream>
#include <memory>
#include "db/fs/CBFS.h"
#include "db/fs/CBBlockEntry.h"
#include "db/fs/bitcoin/CBBlkEntry.h"
#include "db/fs/CBBlockCallback.h"
#include "toolbox/TranslationFun.h"


/*
 * A simple example callback to illustrate block reader usage.
 * */
class MyCallback : public db::fs::CBBlockCallback {
public:

	void operator()(db::fs::CBBlockEntry &cbe) override {
		auto block = cbe.getBlock();
		auto block_hash = block.get_hex();
		std::cout << "BLOCK " <<  block_hash << std::endl;

	}

	void started_file(const boost::filesystem::path &file_path) override {
		std::cout << "WORKING BLOCKS FILE " <<  file_path.generic_string() << std::endl;
	}

	/*
	 * Called every time a block file is fully read.
	 * Can be used to calculate percentage of block files done.
	 *
	 * */
	void done_file(const boost::filesystem::path &file_path) {
		std::cout << "DONE BLOCKS FILE " <<  file_path.generic_string() << std::endl;
	}

};

template class db::fs::bitcoin::CBBitcoinChainReader<MyCallback>;

int main(int argc, char **argv) {

	if (argc != 2) {
		std::cerr << "Usage: fs_block_reader <block_files_path>" << std::endl;
		return EXIT_FAILURE;
	}

	boost::filesystem::path block_dir{argv[1]};
	std::shared_ptr<db::fs::bitcoin::CBBitcoinChainReader<MyCallback>> cbfs = std::make_shared<db::fs::bitcoin::CBBitcoinChainReader<MyCallback>>();

	MyCallback cbc;
	cbfs->read_blocks_callback(block_dir, cbc);

	return EXIT_SUCCESS;
}

