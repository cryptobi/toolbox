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

#include "db/fs/bitcoin/CBBlkReader.h"


namespace db::fs::bitcoin {

	std::vector<CBBlkEntry> CBBlkReader::read_blocks(const boost::filesystem::path &file_path) {

		std::vector<CBBlkEntry> retvec;
		auto p1 = blk_open_autofile(file_path);
		CBufferedFile filein(p1, 2*MAX_BLOCK_SERIALIZED_SIZE, MAX_BLOCK_SERIALIZED_SIZE+8, SER_DISK, CLIENT_VERSION);

		while(!filein.eof()) {
			CBBlkEntry be;
			filein >> be.magic;
			filein >> be.block_size;
			filein >> be.block;
			retvec.push_back(std::move(be));
		}

		return retvec;
	}

	FILE *CBBlkReader::blk_open_autofile(const boost::filesystem::path &file_path) {

		FILE *p1 = fopen(file_path.generic_string().c_str(), "rb");
		if (p1 == NULL) {
				throw std::runtime_error{"cannot open file"};
		}

		return p1;
	}


	void CBBlkReader::read_blocks_callback(const boost::filesystem::path &file_path, db::fs::CBBlockCallback &cb, uint64_t byte_offset_start) {

		auto p1 = blk_open_autofile(file_path.generic_string());
		CBufferedFile filein(p1, 2*MAX_BLOCK_SERIALIZED_SIZE, MAX_BLOCK_SERIALIZED_SIZE+8, SER_DISK, CLIENT_VERSION);
		if (byte_offset_start > 0) {
			filein.SetPos(byte_offset_start);
		}

		CBBlkEntry be;
		model::blockchains::CBBlock cbb;
		std::shared_ptr<CBlock> scbbl = std::make_shared<CBlock>();

		while(!filein.eof()) {
			try {

				scbbl->SetNull();
				cbb.setFileByteOffset(filein.GetPos());
				filein >> be.magic;

				if (be.magic != 0xD9B4BEF9) {
					std::cout << fmt::format("BAD MAGIC NUMBER WHILE READING {} BYTE {} BLOCK {} MAGIC {} BLOCKSIZE {}", file_path.generic_string(), filein.GetPos(), scbbl->GetHash().GetHex(), be.magic, be.block_size) << std::endl;
					continue;
				}

				filein >> be.block_size;
				filein >> *scbbl;
				cbb.setBlock(scbbl);
				be.setBlock(cbb);

				if (be.block_size > MAX_BLOCK_SERIALIZED_SIZE) {
					throw std::overflow_error("READ BLOCK SIZE EXCEEDS MAX_BLOCK_SERIALIZED_SIZE. DATA FILE LIKELY CORRUPTED.");
				}

				cb(be);

			} catch (std::exception &e) {
				std::cout << fmt::format("EXCEPTION READING {} BYTE {} BLOCK {} MAGIC {} BLOCKSIZE {}", file_path.generic_string(), filein.GetPos(), scbbl->GetHash().GetHex(), be.magic, be.block_size) << std::endl;
				std::cout << e.what() << std::endl;
				std::cout << fmt::format("DATA FILE {} MAY BE CORRUPTED", file_path.generic_string()) << std::endl;
			}
		}

	}

	int CBBlkReader::get_last_blockfile_nr(const boost::filesystem::path &file_path) {

		int lastnr = 0;
		boost::filesystem::directory_iterator di{file_path};
		for (const auto &f : di) {
			if (boost::filesystem::is_regular_file(f)) {
				int lcn = ___extract_nr(f.path().stem().generic_string());
				if (lcn > lastnr) {
					lastnr = lcn;
				}
			}
		}

		return lastnr;
	}

	std::vector<boost::filesystem::path> CBBlkReader::generate_block_filenames(const boost::filesystem::path &file_path, const int lastnr) {

		std::vector<boost::filesystem::path> v;
		char nr[32];
		for (int i=0; i<=lastnr; i++) {
			 sprintf(nr, "%05d", i);
			 std::string filename = std::string{"blk"} + std::string{nr} + std::string{".dat"};
			 boost::filesystem::path lp = file_path;
			 lp /= filename;
			 v.push_back(lp);
		}
		return v;
	}



} // namespace


