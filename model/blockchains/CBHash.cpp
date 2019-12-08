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
#include "CBHash.h"

namespace model::blockchains {

	CBHash::CBHash() {
		_hash = new uint256;
	}

	CBHash::~CBHash() {
		delete _hash;
	}

}


