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
#ifndef DB_DAO_MYSQL_CBMYSQL_H_
#define DB_DAO_MYSQL_CBMYSQL_H_

#include <memory>
#include <vector>
#include <iostream>
#include <mysql/jdbc.h>
#include <boost/filesystem.hpp>
#include "toolbox/system/CBConfig.h"
#include <fmt/bundled/format.h>
#include "db/dao/CBDAODriver.h"
#include <primitives/block.h>
#include <uint256.h>
#include <cstdint>
#include <mutex>
#include <cassert>

namespace db::dao::mysql {

struct CBMyConn {
	// had planned one mutex per mysql connection
	// due to random core dumps. seems fixed without the mutexes
	// might remove this struct and use bare sql::Connection *conn
	std::mutex mtx;
	sql::Connection *conn;
};

class CBMySQL: public db::dao::CBDAODriver {

	sql::Driver *driver;
	std::mutex _connection_mutex;
	std::atomic<int> _cons_i;
	uint32_t pool_size = 0;
	std::vector<CBMyConn *> _cons;
	std::mutex _query_mutex;
	std::string dbuser;
	std::string dbhost;
	std::string dbdb;
	std::string dbpass;

	/*
	 * Registry of table names.
	 * */
	std::vector<std::string> table_names {
		"cb_blockchain",
		"cb_blockchain_files",
		"cb_tx",
		"cb_tx_in",
		"cb_tx_out",
		"cb_tx_out_addresses",
		"cb_address_graph",
		"cb_address_balance",
		"cb_info_nodes",
	};


public:

	inline CBMyConn *get_connection(){

		std::scoped_lock<std::mutex> lk{_connection_mutex};
		CBMyConn *ret;

		ret = _cons[_cons_i];
		++_cons_i;
		_cons_i = _cons_i % pool_size;

		assert(ret != nullptr);

		return ret;
	}

	CBMySQL();
	// instantiate driver and automatically connect
	CBMySQL(const std::string &user, const std::string &pass, const std::string &host, const std::string &db);
	std::string test_connection();
	// connect to instance
	bool connect(const std::string &user, const std::string &pass, const std::string &host, const std::string &db) override;
	~CBMySQL();
	bool insert_tx_out_address(const model::blockchains::CBHash &tx_hash, unsigned int nvout,
			const std::string &addr, uint8_t req_sigs,
			uint8_t script_type) override;
	bool list_tx_out_addresses(const model::blockchains::CBHash &tx_hash,
			std::vector<
					std::__cxx11::basic_string<char, std::char_traits<char>,
							std::allocator<char> >,
					std::allocator<
							std::__cxx11::basic_string<char,
									std::char_traits<char>, std::allocator<char> > > > &ret_txout)
					override;
	bool insert_block_file(const boost::filesystem::path &filename,
			const model::blockchains::CBHash &hash, uint32_t byte_offset) override;
	bool get_tx_by_hash(const model::blockchains::CBHash &hash, model::blockchains::CBTx &ret_tx) override;
	bool list_info_node_by_address(const std::string &address)
			override;
	bool list_tx_in(const model::blockchains::CBHash &tx_hash,
			std::vector<model::blockchains::CBTxIn, std::allocator<model::blockchains::CBTxIn> > &ret_txout) override;
	bool insert_block(const model::blockchains::CBBlock &block) override;
	bool get_tx_in(const model::blockchains::CBTxPoint &vin, model::blockchains::CBTxIn &ret_txin) override;
	bool insert_tx_out(const model::blockchains::CBTxOut &tx) override;
	bool list_info_node_by_block_hash(const model::blockchains::CBHash &hash) override;
	bool update_info_node(const CBInfoNode &inode) override;
	bool insert_tx_in(const model::blockchains::CBTxIn &tx) override;
	bool get_tx_out(const model::blockchains::CBTxPoint &vout, model::blockchains::CBTxOut &ret_txout) override;
	bool search_info_node(const std::string &q,
			std::vector<CBInfoNode, std::allocator<CBInfoNode> > &ret_vector)
					override;
	bool list_info_node_by_tx_hash(const model::blockchains::CBHash &hash) override;
	bool insert_tx(const model::blockchains::CBTx &tx) override;
	bool insert_info_node(const CBInfoNode &inode) override;
	bool list_tx_out(const model::blockchains::CBHash &tx_hash,
			std::vector<model::blockchains::CBTxOut, std::allocator<model::blockchains::CBTxOut> > &ret_txout) override;
	bool get_block_by_hash(const model::blockchains::CBHash &hash, model::blockchains::CBBlock &ret_block) override;
	bool insert_address_graph(const model::blockchains::CBTxPoint &vin, const model::blockchains::CBTxPoint &vout, const std::string &addr_from, const std::string &addr_to, const uint64_t satoshis, const uint32_t n_time) override;
	bool get_info_node_by_id(const uint64_t id, CBInfoNode &ret_inode) override;
	bool delete_info_node(const uint64_t id) override;
	bool disable_keys() override;
	bool enable_keys() override;
	bool get_latest_tx(model::blockchains::CBTx &ret_tx) override;
	bool get_latest_tx_in(model::blockchains::CBTxIn &ret_txin) override;
	bool get_latest_block(model::blockchains::CBBlock &ret_block) override;
	bool get_latest_block_file(boost::filesystem::path &filename, model::blockchains::CBHash &hash, uint32_t &byte_offset) override;
	bool get_latest_tx_out(model::blockchains::CBTxOut &ret_txout) override;
	bool get_latest_block_hash(uint256 &ret_hash) override;
};

} // namespace

#endif /* DB_DAO_MYSQL_CBMYSQL_H_ */
