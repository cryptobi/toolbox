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
#include "CBTx.h"

namespace model::blockchains {

	CBTx::~CBTx() {	}

	std::string CBTx::get_hex() {
		return _tx->GetHash().GetHex();
	}

	CBTxIn::CBTxIn() : _txi(nullptr){
	}

	CBTxIn::~CBTxIn() {
	}

	CBTxOut::CBTxOut() : _txo(nullptr) {
	}

	CBTxOut::~CBTxOut() {
	}

	CBTxPoint::CBTxPoint() {
	}

	CBTxPoint::~CBTxPoint() {
	}
}

