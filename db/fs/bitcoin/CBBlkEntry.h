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
#ifndef DB_FS_BITCOIN_CBBLKENTRY_H_
#define DB_FS_BITCOIN_CBBLKENTRY_H_

#include <cstdint>
#include <primitives/block.h>
#include "db/fs/CBBlockEntry.h"
#include "db/fs/bitcoin/CBEncoder.h"

namespace db::fs::bitcoin {

class CBBlkEntry : public db::fs::CBBlockEntry {
	CBEncoder cbe;
public:
	CBBlkEntry();
	CBBlkEntry(CBBlkEntry &e);
	CBBlkEntry(CBBlkEntry &&e);
	~CBBlkEntry();
	uint32_t magic;
   	uint32_t block_size;
   	CBlock block;
};

} /* namespace fs */

#endif /* DB_FS_BITCOIN_CBBLKENTRY_H_ */
