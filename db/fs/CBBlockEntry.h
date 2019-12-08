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
#ifndef DB_FS_CBBLOCKENTRY_H_
#define DB_FS_CBBLOCKENTRY_H_

#include "model/blockchains/CBBlock.h"

namespace db::fs {

class CBBlockEntry {
public:
	CBBlockEntry();
	CBBlockEntry(const model::blockchains::CBBlock _b) : _block(_b) {

	}
	~CBBlockEntry();

	const model::blockchains::CBBlock& getBlock() const {
		return _block;
	}

	void setBlock(const model::blockchains::CBBlock &block) {
		_block = block;
	}

protected:
	model::blockchains::CBBlock _block;

};

} /* namespace fs */

#endif /* DB_FS_CBBLOCKENTRY_H_ */
