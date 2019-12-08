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
 * blocks_to_sql is a blockchain to SQL ETL tool.
 *
 * It uses the schema defined under scripts/sql/mysql to produce a
 * SQL dump file that you can import into your MySQL server.
 *
 *
 */

#include <cstdlib>
#include <boost/filesystem.hpp>
#include <boost/asio/thread_pool.hpp>
#include <iostream>
#include <memory>
#include <cstdint>
#include <chrono>
#include "db/fs/CBFS.h"
#include "model/blockchains/CBBlock.h"
#include "model/blockchains/CBTx.h"
#include "db/dao/CBDAODriver.h"
#include "db/fs/CBBlockEntry.h"
#include "db/fs/bitcoin/CBEncoder.h"
#include "db/fs/bitcoin/CBBlkEntry.h"
#include "db/fs/CBBlockCallback.h"
#include "toolbox/system/CBConfig.h"
#include <fmt/bundled/format.h>
#include "toolbox/TranslationFun.h"

class BitcoinETLCallback : public db::fs::CBBlockCallback {

	boost::filesystem::path current_file;
	std::chrono::system_clock::time_point last_file_time;
	int files_done;
	std::string dbdb;


public:

	uint256 genhash;

	// Genesis block hash is hardcoded
	BitcoinETLCallback() : last_file_time(std::chrono::system_clock::now()), files_done(0) {
		auto config = toolbox::system::CBConfig::get_config();
		genhash.SetHex("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f");
		dbdb   = *config->get_conf("db.db");
	}

	/*
	 * Simple output indirection that can be easily changed later if needed.
	 * For now it just outputs direct to stdout.
	 * */
	void output_sql(const std::string &str){
		std::cout << str << std::endl;
	}

	void operator()(db::fs::CBBlockEntry &cbe) override {


		auto block = cbe.getBlock();
		db::fs::bitcoin::CBEncoder encoder;

		// skip genesis block
		if (genhash != block.getBlock()->GetHash()) {

			std::shared_ptr<model::blockchains::CBHash> cbh = std::make_shared<model::blockchains::CBHash>();
			uint256 phash = block.getBlock()->GetHash();
			cbh->setHash(&phash);
			auto pos = block.getFileByteOffset();

			std::string sql = fmt::format("INSERT INTO {}.cb_blockchain(n_version, hash_this_block, hash_prev_block, hash_merkle_root, n_time, n_bits, nonce);"
					"VALUES({},UNHEX('{}'),UNHEX('{}'),UNHEX('{}'),{},{},{})",
					dbdb, block.getBlock()->nVersion, block.getBlock()->GetHash().GetHex(), block.getBlock()->hashPrevBlock.GetHex(),
					block.getBlock()->hashMerkleRoot.GetHex(), block.getBlock()->nTime, block.getBlock()->nBits, block.getBlock()->nNonce);

			output_sql(sql);

			sql = fmt::format("INSERT INTO {}.cb_blockchain_files(hash_this_block, filename, byte_offset) VALUES(UNHEX('{}'),{},{});",
					dbdb, cbh->getHash()->GetHex(),current_file.generic_string(), pos);

			output_sql(sql);

			auto vtxs = block.getBlock()->vtx;

			int seq = 0;
			for (const auto &txref : vtxs) {

				model::blockchains::CBTx tx(txref);
				tx.setBlockOrder(seq++);
				tx.setBlockHash(phash);

				sql = fmt::format("INSERT INTO {}.cb_tx(hash_this_tx, n_version, has_witness, in_counter, out_counter, lock_time, "
						"block_order, witness_hash, hash_block)"
						"VALUES(UNHEX('{}'),{},{},{},{},{},{},UNHEX('{}'),UNHEX('{}'));",
						dbdb, tx.getTx()->GetHash().GetHex(), tx.getTx()->nVersion, tx.getTx()->HasWitness() ? 1 : 0,
						tx.getTx()->vin.size(), tx.getTx()->vout.size(), tx.getTx()->nLockTime, seq,
						tx.getTx()->GetWitnessHash().GetHex(), block.getBlock()->GetHash().GetHex());

				output_sql(sql);

				int vinseq = 0;
				for (const auto &ivxref: txref->vin) {
					model::blockchains::CBTxIn txin;
					txin.setHashThisTx(txref->GetHash());
					txin.setThisSeq(vinseq++);
					auto ptxin = const_cast<CTxIn *>(&ivxref);
					txin.setTxi(ptxin);

					sql = fmt::format("INSERT INTO {}.cb_tx_in(txid, n_vout, hash_this_tx, n_sequence) "
							"VALUES(UNHEX('{}'),{},UNHEX('{}'),{});",
							dbdb, ivxref.prevout.hash.GetHex(), ivxref.prevout.n, txref->GetHash().GetHex(),
							vinseq);

					output_sql(sql);

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

					sql = fmt::format("INSERT INTO {}.cb_tx_out(satoshis, n_vout, hash_this_tx, hash_tx_spent, script_req_sigs, script_type) "
							"VALUES({}, {}, UNHEX('{}'),UNHEX('{}'),{},{});",
							dbdb, txout.getTxo()->nValue, voutseq, txout.getHashThisTx().GetHex(), uint256().GetHex(),
							txout.getTxo()->scriptPubKey.GetSigOpCount(txout.getTxo()->scriptPubKey), script_type);

					output_sql(sql);

					model::blockchains::CBHash tx_hash;
					uint256 txh = txref->GetHash();
					tx_hash.setHash(&txh);

					std::vector<CTxDestination> addressRet;
					int nRequiredRet;
					txnouttype typeRet;
					encoder.ExtractDestinations(ovxref.scriptPubKey, typeRet, addressRet, nRequiredRet);

					for (const auto &addr: addressRet) {
						std::string encoded_address = encoder.EncodeDestination(addr);

						sql = fmt::format("INSERT INTO {}.cb_tx_out_addresses(n_vout, hash_tx, address, script_req_sigs, script_type) "
								"VALUES({},UNHEX('{}'),{},{},{});",
								dbdb, voutseq, tx_hash.getHash()->GetHex(), encoded_address, ovxref.scriptPubKey.GetSigOpCount(ovxref.scriptPubKey), script_type);

						output_sql(sql);
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
		std::cerr << fmt::format("DONE BLOCKS FILE {} IN {}s. {} FILES DONE ", file_path.generic_string(), delta.count(), files_done) << std::endl;
	}

};

template class db::fs::bitcoin::CBBitcoinChainReader<BitcoinETLCallback>;

int main(int argc, char **argv) {

	std::string blocks_dir_name;
	auto config = toolbox::system::CBConfig::get_config();
	config->init_config(argc, argv);

	BitcoinETLCallback cbc;

	auto apds = config->get_conf("bitcoin_blocks_dir");

	if ( argc == 2 ) {
		blocks_dir_name = std::string{ *(argv + 1)};
	} else if (apds) {
		blocks_dir_name = *(apds);
	}

	boost::filesystem::path block_dir{blocks_dir_name};
	std::shared_ptr<db::fs::bitcoin::CBBitcoinChainReader<BitcoinETLCallback>> cbfs = std::make_shared<db::fs::bitcoin::CBBitcoinChainReader<BitcoinETLCallback>>();


	cbfs->read_blocks_callback(block_dir, cbc);

	return EXIT_SUCCESS;
}

