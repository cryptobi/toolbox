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

#ifndef DB_DAO_CBDAODRIVER_H_
#define DB_DAO_CBDAODRIVER_H_

#include <string>
#include <vector>
#include <cstdint>
#include <boost/filesystem.hpp>
#include <uint256.h>
#include "model/blockchains/CBBlock.h"
#include "model/blockchains/CBHash.h"
#include "model/blockchains/CBTx.h"
#include "model/graph/CBInfoNode.h"

namespace db::dao {

	class CBDAODriver {
	public:
		CBDAODriver() {};
		virtual bool connect(const std::string &user, const std::string &pass, const std::string &host, const std::string &db) = 0;
		virtual ~CBDAODriver();
		virtual std::string test_connection() = 0;
		virtual bool insert_block(const model::blockchains::CBBlock &block) = 0;
		virtual bool get_block_by_hash(const model::blockchains::CBHash &hash, model::blockchains::CBBlock &ret_block) = 0;
		virtual bool get_latest_block(model::blockchains::CBBlock &ret_block) = 0;
		virtual bool get_latest_block_hash(uint256 &ret_hash) = 0;
		virtual bool insert_block_file(const boost::filesystem::path &filename, const model::blockchains::CBHash &hash, uint32_t byte_offset) = 0;
		virtual bool get_latest_block_file(boost::filesystem::path &filename, model::blockchains::CBHash &hash, uint32_t &byte_offset) = 0;
		virtual bool insert_tx(const model::blockchains::CBTx &tx) = 0;
		virtual bool get_tx_by_hash(const model::blockchains::CBHash &hash, model::blockchains::CBTx &ret_tx) = 0;
		virtual bool get_latest_tx(model::blockchains::CBTx &ret_tx) = 0;
		virtual bool insert_tx_in(const model::blockchains::CBTxIn &tx) = 0;
		virtual bool get_tx_in(const model::blockchains::CBTxPoint &vin, model::blockchains::CBTxIn &ret_txin) = 0;
		virtual bool get_latest_tx_in(model::blockchains::CBTxIn &ret_txin) = 0;
		virtual bool list_tx_in(const model::blockchains::CBHash &tx_hash, std::vector<model::blockchains::CBTxIn> &ret_txin) = 0;
		virtual bool insert_tx_out(const model::blockchains::CBTxOut &tx) = 0;
		virtual bool get_tx_out(const model::blockchains::CBTxPoint &vout, model::blockchains::CBTxOut &ret_txout) = 0;
		virtual bool get_latest_tx_out(model::blockchains::CBTxOut &ret_txout) = 0;
		virtual bool list_tx_out(const model::blockchains::CBHash &tx_hash, std::vector<model::blockchains::CBTxOut> &ret_txout) = 0;
		virtual bool insert_tx_out_address(const model::blockchains::CBHash &tx_hash, const uint32_t nvout, const std::string &addr, uint8_t req_sigs, uint8_t script_type) = 0;
		virtual bool list_tx_out_addresses(const model::blockchains::CBHash &tx_hash, std::vector<std::string> &ret_txout) = 0;
		virtual bool insert_address_graph(const model::blockchains::CBTxPoint &vin, const model::blockchains::CBTxPoint &vout, const std::string &addr_from, const std::string &addr_to, const uint64_t satoshis, const uint32_t n_time) = 0;
		virtual bool insert_info_node(const CBInfoNode &inode) = 0;
		virtual bool get_info_node_by_id(const uint64_t id, CBInfoNode &ret_inode) = 0;
		virtual bool list_info_node_by_address(const std::string &address) = 0;
		virtual bool list_info_node_by_block_hash(const model::blockchains::CBHash &hash) = 0;
		virtual bool list_info_node_by_tx_hash(const model::blockchains::CBHash &hash) = 0;
		virtual bool search_info_node(const std::string &q, std::vector<CBInfoNode> &ret_vector) = 0;
		virtual bool update_info_node(const CBInfoNode &inode) = 0;
		virtual bool delete_info_node(const uint64_t id) = 0;
		virtual bool disable_keys() = 0;
		virtual bool enable_keys() = 0;

	};

}


#endif /* DB_DAO_CBDAODRIVER_H_ */
