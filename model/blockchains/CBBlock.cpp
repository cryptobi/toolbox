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
#include "CBBlock.h"

namespace model::blockchains {

	CBBlock::CBBlock(): magic(0), block_size(0), _block(nullptr), file_byte_offset(0) {
	}

	CBBlock::~CBBlock() {
	}

	std::string CBBlock::get_hex() const {
		return _block->GetHash().GetHex();
	}

}


