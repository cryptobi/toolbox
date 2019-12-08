
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

#include "CBBlkEntry.h"

namespace db::fs::bitcoin {

	CBBlkEntry::CBBlkEntry(): magic(0), block_size(0) {

	}

	CBBlkEntry::CBBlkEntry(CBBlkEntry &&e): magic(e.magic), block_size(e.block_size), block(e.block) {
	}

	CBBlkEntry::~CBBlkEntry() {

	}

	CBBlkEntry::CBBlkEntry(CBBlkEntry &e): magic(e.magic), block_size(e.block_size), block(e.block) {

	}

}


