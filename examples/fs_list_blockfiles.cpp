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

// Lists block data files in Bitcoin Core blocks/ directory.
// Shows usage of db::fs::bitcoin::CBBlkReader static functions
// Does not simply read the filenames from the filesystem.
// It finds the largest block file index number N
// and then generates block file names starting from N.

#include "db/fs/CBFS.h"
#include "db/fs/bitcoin/CBBlkReader.h"
#include "toolbox/TranslationFun.h"

int main(int argc, char **argv) {

	if (argc != 2) {
		std::cerr << "Usage: fs_list_blockfiles <block_files_path>" << std::endl;
		return EXIT_FAILURE;
	}

	std::string blocks_dir = std::string{argv[1]};
	boost::filesystem::path file_path{blocks_dir};
	int nr = db::fs::bitcoin::CBBlkReader::get_last_blockfile_nr(file_path);
	std::cout << nr << std::endl;
	auto files = db::fs::bitcoin::CBBlkReader::generate_block_filenames(file_path, nr);

	for (const auto &file: files) {
		std::cout << file.generic_string() << std::endl;
	}

	return EXIT_SUCCESS;
}
