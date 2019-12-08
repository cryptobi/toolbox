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
#ifndef MODEL_CBHASH_H_
#define MODEL_CBHASH_H_

#include <uint256.h>
#include <memory>

namespace model::blockchains {


class CBHash {
	uint256 *_hash;
public:
	CBHash();

	CBHash(uint256 * h): _hash(h){

	}

	virtual ~CBHash();

	const uint256 * getHash() const {
		return _hash;
	}

	void setHash(uint256 * hash) {
		_hash = hash;
	}

	void setHash(uint256 hash) {
		*_hash = hash;
	}

};

}

#endif /* MODEL_CBHASH_H_ */

