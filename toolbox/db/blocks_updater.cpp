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
 * blocks_updater: Worker process that keeps blockchain in sync with local relational database.
 * This is a slow running background process.
 *
 */

#include <cstdlib>
#include <boost/filesystem.hpp>
#include <iostream>
#include <memory>
#include <cstdint>
#include <string>
#include <chrono>
#include "db/fs/CBFS.h"
#include "db/dao/CBDAO.h"
#include "model/blockchains/CBBlock.h"
#include "model/blockchains/CBTx.h"
#include "db/dao/CBDAODriver.h"
#include "db/fs/CBBlockEntry.h"
#include "db/fs/bitcoin/CBEncoder.h"
#include "db/fs/bitcoin/CBBlkEntry.h"
#include "db/fs/CBBlockCallback.h"
#include "toolbox/system/CBConfig.h"
#include <fmt/bundled/format.h>
#include <boost/program_options.hpp>
#include <boost/lexical_cast.hpp>
#include <sstream>
#include "toolbox/TranslationFun.h"
#include "db/fs/bitcoin/CBBlkReader.h"

namespace po = boost::program_options;

class BlockUpdaterCallback : public db::fs::CBBlockCallback {

	db::dao::CBDAODriver *dao = db::dao::CBDAO::get_DAO();
	boost::filesystem::path current_file;
	std::chrono::system_clock::time_point last_file_time;
	int files_done;
	boost::filesystem::path working_file;
	model::blockchains::CBHash working_hash;
	uint32_t working_byte_offset;
	std::shared_ptr<toolbox::system::CBConfig> config;


public:

	// Genesis block hash is hardcoded
	BlockUpdaterCallback() : last_file_time(std::chrono::system_clock::now()),
			files_done(0), working_byte_offset(0), config(nullptr) {
		dao->get_latest_block_file(working_file, working_hash, working_byte_offset);
	}

	void operator()(db::fs::CBBlockEntry &cbe) override {

		auto block = cbe.getBlock();
		db::fs::bitcoin::CBEncoder encoder;

		auto b2 = block.getBlock();

		if (b2 != nullptr) {

			model::blockchains::CBHash cbh;
			cbh.setHash(b2->GetHash());
			auto pos = block.getFileByteOffset();

			try {
				auto ret  = dao->insert_block(block);
			} catch (sql::SQLException &e) {
				if (config) {
					config->log_error(fmt::format("Error inserting block {}", block.getBlock()->GetHash().GetHex()));
					config->log_error(e.what());
				}
			}

			try {
				auto ret2 = dao->insert_block_file(current_file, cbh, pos);
			} catch (sql::SQLException &e) {
				if (config) {
					config->log_error(fmt::format("Error inserting block file {}", current_file.generic_string()));
					config->log_error(e.what());
				}
			}

			auto vtxs = block.getBlock()->vtx;
			int seq = 0;

			for (CTransactionRef &txref : vtxs) {

				model::blockchains::CBTx tx(txref);
				tx.setBlockOrder(seq++);
				tx.setBlockHash(b2->GetHash());

				try {
					dao->insert_tx(tx);
				} catch (sql::SQLException &e) {
					if (config) {
						config->log_error(fmt::format("Error inserting TX {}", txref->GetHash().GetHex()));
						config->log_error(e.what());
					}
				}

				int vinseq = 0;
				for (const auto &ivxref: txref->vin) {
					model::blockchains::CBTxIn txin;
					txin.setHashThisTx(txref->GetHash());
					txin.setThisSeq(vinseq++);
					auto ptxin = const_cast<CTxIn *>(&ivxref);
					txin.setTxi(ptxin);

					try {
						dao->insert_tx_in(txin);
					} catch (sql::SQLException &e) {
						if (config) {
							config->log_error(fmt::format("Error inserting TX_IN {} SEQ {}", txref->GetHash().GetHex(), vinseq));
							config->log_error(e.what());
						}
					}

				}

				int voutseq = 0;
				for (const auto &ovxref: txref->vout) {

					int script_type = 0;
					if (ovxref.scriptPubKey.IsPayToScriptHash())
						script_type = 1;
					else if (ovxref.scriptPubKey.IsPayToWitnessScriptHash())
						script_type = 3;
					else
						script_type = 2; // nonstandard

					model::blockchains::CBTxOut txout;
					txout.setHashThisTx(txref->GetHash());
					txout.setThisSeq(voutseq);
					auto ptxout = const_cast<CTxOut *>(&ovxref);
					txout.setTxo(ptxout);

					try {
						dao->insert_tx_out(txout);
					} catch (sql::SQLException &e) {
						if (config) {
							config->log_error(fmt::format("Error inserting TX_OUT TX {} SEQ {}", txref->GetHash().GetHex() , voutseq));
							config->log_error(e.what());
						}
					}

					model::blockchains::CBHash tx_hash;
					tx_hash.setHash(txref->GetHash());

					std::vector<CTxDestination> addressRet;
					int nRequiredRet;
					txnouttype typeRet;

					try {
						encoder.ExtractDestinations(ovxref.scriptPubKey, typeRet, addressRet, nRequiredRet);
					} catch (...) {
						config->log_error(fmt::format("Error EXTRACTING DESTINATION ADDRESSES TX {} SEQ {}", txref->GetHash().GetHex() , voutseq));
					}


					for (const auto &addr: addressRet) {
						std::string encoded_address = encoder.EncodeDestination(addr);

						try {
							dao->insert_tx_out_address(tx_hash, voutseq, encoded_address, ovxref.scriptPubKey.GetSigOpCount(ovxref.scriptPubKey), script_type);
						} catch (sql::SQLException &e) {
							if (config) {
								config->log_error(fmt::format("Error inserting TX_OUT ADDRESS TX {} SEQ {} ADDR {}", txref->GetHash().GetHex() , voutseq, encoded_address));
								config->log_error(e.what());
							}
						}
					}

					voutseq++;
				}
			}
		}
	}

	void started_file(const boost::filesystem::path &file_path) override {
		current_file = file_path;
	}

	void done_file(const boost::filesystem::path &file_path) override {
		files_done++;
		auto config = toolbox::system::CBConfig::get_config();
		auto time_now = std::chrono::system_clock::now();
		std::chrono::duration<double> delta = time_now - last_file_time;
		last_file_time = std::chrono::system_clock::now();
		config->log_message(fmt::format("DONE BLOCKS FILE {} IN {}s {} ITERATIONS DONE ", file_path.generic_string(), delta.count(), files_done));
	}

	const std::shared_ptr<toolbox::system::CBConfig>& getConfig() const {
		return config;
	}

	void setConfig(const std::shared_ptr<toolbox::system::CBConfig> &config) {
		this->config = config;
	}

};

template class db::fs::bitcoin::CBBitcoinChainReader<BlockUpdaterCallback>;

int main(int argc, char **argv) {

	std::string blocks_dir_name;
	auto config = toolbox::system::CBConfig::get_config();
	boost::filesystem::path last_file;

	config->init_config(argc, argv);

	db::dao::CBDAODriver *dao = db::dao::CBDAO::get_DAO();

	if ( dao == nullptr ) {
		config->log_error("Unable to get an instance of the database access object. Check your credentials/configuration and try again.");
		std::exit(1);
	}

	auto apds = config->get_conf("bitcoin_blocks_dir");

	if ( apds ) {
		blocks_dir_name = *(apds);
	} else if (apds) {
		config->log_error("Block data file directory (bitcoin_blocks_dir) must be specified via --bitcoin_blocks_dir or config file.");
		std::exit(1);
	}

	boost::filesystem::path file_path{blocks_dir_name};
	auto files = db::fs::bitcoin::CBBlkReader::list_block_filenames(file_path);
	last_file = files[files.size() - 1];

	std::cout << " last file " << last_file.string() << std::endl;


	/*
	 * Run first time.
	 * */
	config->log_message("Starting blocks updater. (This may take a few minutes.)");
    // get default blocks dir from config


	boost::filesystem::path block_dir{blocks_dir_name};
	std::shared_ptr<db::fs::bitcoin::CBBitcoinChainReader<BlockUpdaterCallback>> cbfs = std::make_shared<db::fs::bitcoin::CBBitcoinChainReader<BlockUpdaterCallback>>();

	if (cbfs == nullptr) {
		config->log_error("Error creating blockchain reader.");
		std::exit(1);
	}


	cbfs->setSkipUntil(last_file);


	BlockUpdaterCallback callback;
	cbfs->read_blocks_callback(block_dir, callback);

	/*
	 * Enter updater loop
	 * */
	while (true) {
		// go to latest data file
		config->log_message(fmt::format("Processing file {}", last_file.string()));
		cbfs->setSkipUntil(last_file);
		cbfs->read_blocks_callback(block_dir, callback);
	}


	return EXIT_SUCCESS;
}

