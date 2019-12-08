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
#include "CBMySQL.h"

namespace db::dao::mysql {

CBMySQL::CBMySQL() {

	auto config = toolbox::system::CBConfig::get_config();

	dbuser = *config->get_conf("db.user");
	dbpass = *config->get_conf("db.pass");
	dbdb   = *config->get_conf("db.db");
	dbhost = *config->get_conf("db.host");

	driver = sql::mysql::get_driver_instance();
	driver->threadInit();

	_cons_i = 0;

	connect(dbuser, dbpass, dbhost, dbdb);
}

CBMySQL::CBMySQL(const std::string &user, const std::string &pass, const std::string &host, const std::string &db) {
	CBMySQL();
	connect(user, pass, host, db);
}

CBMySQL::~CBMySQL() {

	for (int i=0; i<_cons.size(); i++) {
		_cons[i]->conn->close();
		delete _cons[i];
	}

	driver->threadEnd();
}

bool CBMySQL::insert_tx_out_address(const model::blockchains::CBHash &tx_hash, unsigned int nvout, const std::string &addr, uint8_t req_sigs, uint8_t script_type) {

	const static std::string sql = fmt::format("INSERT INTO {}.cb_tx_out_addresses(n_vout, hash_tx, address, script_req_sigs, script_type) VALUES(?,UNHEX(?),?,?,?)", dbdb);


	auto cb_conn = get_connection();
	assert(cb_conn != nullptr);
	std::scoped_lock<std::mutex> lk{cb_conn->mtx};
	auto ps = cb_conn->conn->prepareStatement(sql);
	ps->setInt(1, nvout);
	ps->setString(2, tx_hash.getHash()->GetHex());
	ps->setString(3, addr);
	ps->setInt(4, req_sigs);
	ps->setInt(5, script_type);
	ps->executeUpdate();

	delete ps;

	return true;
}

bool CBMySQL::list_tx_out_addresses(const model::blockchains::CBHash &tx_hash, std::vector<std::string> &ret_txout) {
	return false;
}

bool CBMySQL::insert_block_file(const boost::filesystem::path &filename, const model::blockchains::CBHash &hash, uint32_t byte_offset) {

	const static std::string sql = fmt::format("INSERT INTO {}.cb_blockchain_files(hash_this_block, filename, byte_offset) VALUES(UNHEX(?),?,?)", dbdb);

	auto cb_conn = get_connection();
	assert(cb_conn != nullptr);
	std::scoped_lock<std::mutex> lk{cb_conn->mtx};
	auto ps = cb_conn->conn->prepareStatement(sql);
	ps->setString(1, hash.getHash()->GetHex());
	ps->setString(2, filename.generic_string());
	ps->setInt(3, byte_offset);
	ps->executeUpdate();

	delete ps;

	return true;
}

bool CBMySQL::get_tx_by_hash(const model::blockchains::CBHash &hash, model::blockchains::CBTx &ret_tx) {
	return false;
}

bool CBMySQL::list_info_node_by_address(const std::string &address) {
	return false;
}

bool CBMySQL::list_tx_in(const model::blockchains::CBHash &tx_hash,
		std::vector<model::blockchains::CBTxIn, std::allocator<model::blockchains::CBTxIn> > &ret_txout) {
	return false;
}

bool CBMySQL::insert_block(const model::blockchains::CBBlock &block) {

	auto cblock = block.getBlock();
	const static std::string sql = fmt::format("INSERT INTO {}.cb_blockchain(n_version, hash_this_block, hash_prev_block, hash_merkle_root, n_time, n_bits, nonce) VALUES(?,UNHEX(?),UNHEX(?),UNHEX(?),?,?,?)", dbdb);

	auto cb_conn = get_connection();
	assert(cb_conn != nullptr);
	std::scoped_lock<std::mutex> lk{cb_conn->mtx};
	auto ps = cb_conn->conn->prepareStatement(sql);
	ps->setInt(1, cblock->nVersion);
	ps->setString(2, cblock->GetHash().GetHex());
	ps->setString(3, cblock->hashPrevBlock.GetHex());
	ps->setString(4, cblock->hashMerkleRoot.GetHex());
	ps->setInt(5, cblock->nTime);
	ps->setInt(6, cblock->nBits);
	ps->setInt(7, cblock->nNonce);
	ps->executeUpdate();

	delete ps;

	return true;
}

bool CBMySQL::get_tx_in(const model::blockchains::CBTxPoint &vin, model::blockchains::CBTxIn &ret_txin) {
	return false;
}

bool CBMySQL::insert_tx_out(const model::blockchains::CBTxOut &tx) {

	const static std::string sql = fmt::format("INSERT INTO {}.cb_tx_out(satoshis, n_vout, hash_this_tx, hash_tx_spent, script_req_sigs, script_type) VALUES(?, ?, UNHEX(?),UNHEX(?),?,?)", dbdb);
	const CTxOut *txout = tx.getTxo();

	int script_type = 0;
	if (txout->scriptPubKey.IsPayToScriptHash())
		script_type = 1;
	else if (txout->scriptPubKey.IsPayToWitnessScriptHash())
		script_type = 3;
	else
		script_type = 2; // nonstandard

	auto cb_conn = get_connection();
	assert(cb_conn != nullptr);
	std::scoped_lock<std::mutex> lk{cb_conn->mtx};
	auto ps = cb_conn->conn->prepareStatement(sql);
	ps->setInt64(1, txout->nValue);
	ps->setInt(2, tx.getThisSeq());
	ps->setString(3, tx.getHashThisTx().GetHex());
	ps->setNull(4, 0);
	ps->setInt(5, txout->scriptPubKey.GetSigOpCount(txout->scriptPubKey));
	ps->setInt(6, script_type);
	ps->executeUpdate();

	delete ps;

	return true;
}

bool CBMySQL::list_info_node_by_block_hash(const model::blockchains::CBHash &hash) {
	return false;
}

bool CBMySQL::update_info_node(const CBInfoNode &inode) {
	return false;
}

bool CBMySQL::insert_tx_in(const model::blockchains::CBTxIn &tx) {


	const static std::string sql = fmt::format("INSERT INTO {}.cb_tx_in(txid, n_vout, hash_this_tx, n_sequence) VALUES(UNHEX(?),?,UNHEX(?),?)", dbdb);
	const CTxIn *txin = tx.getTxi();

	auto cb_conn = get_connection();
	assert(cb_conn != nullptr);
	std::scoped_lock<std::mutex> lk{cb_conn->mtx};

	auto ps = cb_conn->conn->prepareStatement(sql);
	ps->setString(1, txin->prevout.hash.GetHex());
	ps->setInt(2, txin->prevout.n);
	ps->setString(3, tx.getHashThisTx().GetHex());
	ps->setInt(4, tx.getThisSeq());
	ps->executeUpdate();

	delete ps;

	return true;
}

bool CBMySQL::get_tx_out(const model::blockchains::CBTxPoint &vout, model::blockchains::CBTxOut &ret_txout) {
	return false;
}

bool CBMySQL::search_info_node(const std::string &q,
		std::vector<CBInfoNode, std::allocator<CBInfoNode> > &ret_vector) {
	return false;
}

bool CBMySQL::list_info_node_by_tx_hash(const model::blockchains::CBHash &hash) {
	return false;
}

bool CBMySQL::insert_tx(const model::blockchains::CBTx &tx) {

	auto ctx = tx.getTx();
	int dbg=0;
	const static std::string sql = fmt::format("INSERT INTO {}.cb_tx(hash_this_tx, n_version, has_witness, in_counter, out_counter, lock_time, block_order, witness_hash, hash_block) VALUES(UNHEX(?),?,?,?,?,?,?,UNHEX(?),UNHEX(?))", dbdb);

	auto cb_conn = get_connection();
	assert(cb_conn != nullptr);
	std::scoped_lock<std::mutex> lk{cb_conn->mtx};

	auto ps = cb_conn->conn->prepareStatement(sql);
	ps->setString(1, ctx->GetHash().GetHex());
	ps->setInt(2, ctx->nVersion);
	ps->setInt(3, ctx->HasWitness() ? 1 : 0);
	ps->setInt(4, ctx->vin.size());
	ps->setInt(5, ctx->vout.size());
	ps->setInt(6, ctx->nLockTime);
	ps->setInt(7, tx.getBlockOrder());
	ps->setString(8, ctx->GetWitnessHash().GetHex());
	ps->setString(9, tx.getBlockHash().GetHex());
	ps->executeUpdate();

	delete ps;

	return true;
}

bool CBMySQL::insert_info_node(const CBInfoNode &inode) {
	return false;
}

bool CBMySQL::list_tx_out(const model::blockchains::CBHash &tx_hash,
		std::vector<model::blockchains::CBTxOut, std::allocator<model::blockchains::CBTxOut> > &ret_txout) {
	return false;
}

std::string CBMySQL::test_connection() {

	auto cb_conn = get_connection();
	assert(cb_conn != nullptr);
	std::scoped_lock<std::mutex> lk{cb_conn->mtx};

	bool ok = false;
	std::string ret;
	const static std::string sql = fmt::format("SELECT VERSION() AS _v", dbdb);
	auto ps = cb_conn->conn->prepareStatement(sql);
	auto rs = ps->executeQuery();
	if (rs->next()) {
		ret = rs->getString(1);
		ok = true;
	}

	delete rs;
	delete ps;

	if (ok)
		return ret;
	else
		return "ERROR";
}

bool CBMySQL::get_block_by_hash(const model::blockchains::CBHash &hash, model::blockchains::CBBlock &ret_block) {
	return false;
}

bool CBMySQL::insert_address_graph(const model::blockchains::CBTxPoint &vin,
		const model::blockchains::CBTxPoint &vout, const std::string &addr_from,
		const std::string &addr_to, const uint64_t satoshis,
		const uint32_t n_time) {
	return false;
}

bool CBMySQL::get_info_node_by_id(const uint64_t id, CBInfoNode &ret_inode) {
	return false;
}

bool CBMySQL::delete_info_node(const uint64_t id) {
	return false;
}

bool CBMySQL::disable_keys() {

	auto cb_conn = get_connection();
	assert(cb_conn != nullptr);
	std::scoped_lock<std::mutex> lk{cb_conn->mtx};

	auto stmt = cb_conn->conn->createStatement();
	for (const auto &t: table_names) {
		stmt->executeUpdate(fmt::format("ALTER TABLE {} DISABLE KEYS", t));
	}

	delete stmt;
	return true;
}

bool CBMySQL::enable_keys() {

	auto cb_conn = get_connection();
	assert(cb_conn != nullptr);
	std::scoped_lock<std::mutex> lk{cb_conn->mtx};

	auto stmt = cb_conn->conn->createStatement();
	for (const auto &t: table_names) {
		stmt->executeUpdate(fmt::format("ALTER TABLE {} ENABLE KEYS", t));
	}

	delete stmt;
	return true;
}

bool CBMySQL::get_latest_tx(model::blockchains::CBTx &ret_tx) {
	return false;
}

bool CBMySQL::get_latest_tx_in(model::blockchains::CBTxIn &ret_txin) {
	return false;
}

bool CBMySQL::get_latest_block(model::blockchains::CBBlock &ret_block) {

	const static std::string sql = fmt::format("SELECT * FROM {}.cb_blockchain ORDER BY table_seq DESC LIMIT 1", dbdb);


	auto cb_conn = get_connection();
	assert(cb_conn != nullptr);
	std::scoped_lock<std::mutex> lk{cb_conn->mtx};

	auto stmt = cb_conn->conn->createStatement();
	auto rs = stmt->executeQuery(sql);
	if (rs->next()) {
		ret_block.setTableSeq(rs->getInt64("table_seq"));
		// TODO here we'd directly access Bitcoin Core
		// CBlock file. Waiting to do this later. For now will
		// use a simpler version getting just the block hash.
		return true;
	}

	delete rs;
	delete stmt;
	return false;
}

bool CBMySQL::get_latest_block_file(boost::filesystem::path &filename,	model::blockchains::CBHash &hash, uint32_t &byte_offset) {

	const static std::string sql = fmt::format("SELECT table_seq, HEX(hash_this_block), filename, byte_offset FROM {}.cb_blockchain_files ORDER BY table_seq DESC LIMIT 1", dbdb);

	auto cb_conn = get_connection();
	assert(cb_conn != nullptr);
	std::scoped_lock<std::mutex> lk{cb_conn->mtx};

	auto stmt = cb_conn->conn->createStatement();
	auto rs = stmt->executeQuery(sql);

	if (rs->next()) {
		filename = boost::filesystem::path{rs->getString(3)};
		uint256 *ui2 = new uint256;
		ui2->SetHex(rs->getString(2));
		hash.setHash(ui2);
		byte_offset = rs->getUInt(4);
		return true;
	}

	delete rs;
	delete stmt;
	return false;
}

bool CBMySQL::get_latest_tx_out(model::blockchains::CBTxOut &ret_txout) {
	return false;
}

bool CBMySQL::get_latest_block_hash(uint256 &ret_hash) {

	const static std::string sql = fmt::format("SELECT HEX(hash_this_block) FROM {}.cb_blockchain ORDER BY table_seq DESC LIMIT 1", dbdb);


	auto cb_conn = get_connection();
	assert(cb_conn != nullptr);
	std::scoped_lock<std::mutex> lk{cb_conn->mtx};

	auto stmt = cb_conn->conn->createStatement();
	auto rs = stmt->executeQuery(sql);

	if (rs->next()) {
		ret_hash.SetHex(rs->getString(1));
		return true;
	}

	delete rs;
	delete stmt;
	return false;
}

bool CBMySQL::connect(const std::string &user, const std::string &pass, const std::string &host, const std::string &db) {

	// pool_size = std::thread::hardware_concurrency();
	// i've parallelized DAO, so opening multiple connections
	// here would create n * n. leave at 1 for now.
	pool_size = 1;

	_cons.reserve(pool_size);

	for (int i=0; i<pool_size; i++) {
		_cons[i] = new CBMyConn();
		_cons[i]->conn = driver->connect(host, user, pass);
		_cons[i]->conn->setSchema(sql::SQLString{db});
	}

	return true;
}

} // namespace


