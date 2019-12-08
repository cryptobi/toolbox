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
#ifndef MODEL_CBTX_H_
#define MODEL_CBTX_H_

#include <memory>
#include <string>
#include <vector>
#include <primitives/transaction.h>

namespace model::blockchains {

class CBTx {

	CTransactionRef _tx;
	std::string get_hex();
	int block_order;
	uint256 block_hash;
	uint64_t table_seq; // toolbox sequential nr, not on blockchain


public:

	CBTx(): _tx(nullptr), block_order(0), block_hash(uint256()), table_seq(0){

	}

	CBTx(const CTransactionRef t): _tx(t), block_order(0), block_hash(uint256()), table_seq(0){

	}

	CBTx(CTransactionRef t, int bo, uint256 bh):_tx(t), block_order(bo), block_hash(bh), table_seq(0){

	}

	virtual ~CBTx();

	const CTransactionRef &getTx() const {
		return _tx;
	}

	int getBlockOrder() const {
		return block_order;
	}

	void setBlockOrder(int blockOrder) {
		block_order = blockOrder;
	}

	const uint256& getBlockHash() const {
		return block_hash;
	}

	void setBlockHash(const uint256 &blockHash) {
		block_hash = blockHash;
	}
};

class CBTxIn {
	CTxIn * _txi;
	uint256 hash_this_tx;
	int this_seq;
	uint64_t table_seq; // toolbox sequential nr, not on blockchain
public:
	CBTxIn();
	CBTxIn(CTxIn * t): _txi(t), hash_this_tx(uint256{}), this_seq(0), table_seq(0){

	}
	virtual ~CBTxIn();

	const CTxIn *getTxi() const {
		return _txi;
	}

	void setTxi( CTxIn *txi) {
		_txi = txi;
	}

	const uint256& getHashThisTx() const {
		return hash_this_tx;
	}

	void setHashThisTx(const uint256 &hashThisTx) {
		hash_this_tx = hashThisTx;
	}

	int getThisSeq() const {
		return this_seq;
	}

	void setThisSeq(int thisSeq) {
		this_seq = thisSeq;
	}
};

class CBTxOut {
	CTxOut * _txo;
	uint256 hash_this_tx;
	int this_seq;
	uint64_t table_seq; // toolbox sequential nr, not on blockchain

public:
	CBTxOut();
	CBTxOut(CTxOut * t): _txo(t), hash_this_tx(uint256()), this_seq(0), table_seq(0) {

	}
	virtual ~CBTxOut();

	const CTxOut * getTxo() const {
		return _txo;
	}

	void setTxo(const CTxOut * txo) {
		_txo = const_cast<CTxOut *>(txo);
	}

	const uint256& getHashThisTx() const {
		return hash_this_tx;
	}

	void setHashThisTx(const uint256 &hashThisTx) {
		hash_this_tx = hashThisTx;
	}

	int getThisSeq() const {
		return this_seq;
	}

	void setThisSeq(int thisSeq) {
		this_seq = thisSeq;
	}
};

class CBTxPoint {
	COutPoint *_op;

public:
	CBTxPoint();
	CBTxPoint(COutPoint *o): _op(o) {

	}
	virtual ~CBTxPoint();

	const COutPoint * getOp() const {
		return _op;
	}

	void setOp(const COutPoint *op) {
		_op = const_cast<COutPoint *>(op);
	}
};

}

#endif /* MODEL_CBTX_H_ */
