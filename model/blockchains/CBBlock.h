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
#ifndef MODEL_CBBLOCK_H_
#define MODEL_CBBLOCK_H_

#include <memory>
#include <primitives/block.h>
#include "model/blockchains/CBHash.h"
#include "CBTx.h"

namespace model::blockchains {

class CBBlock {

	uint64_t table_seq; // toolbox sequential nr, not on blockchain
	uint32_t magic;
   	uint32_t block_size;
	std::shared_ptr<CBlock> _block;
	std::vector<CBTx> vtx;
	uint64_t file_byte_offset;

public:
	CBBlock(const uint32_t mg, const uint32_t bsz, const std::shared_ptr<CBlock> &_blk, const uint64_t &offset):
		table_seq(0), magic(mg), block_size(bsz), _block(_blk), file_byte_offset(offset) {

	}
	CBBlock();
	virtual ~CBBlock();

	const std::shared_ptr<CBlock>& getBlock() const {
		return _block;
	}

	void setBlock(const std::shared_ptr<CBlock> &block) {
		_block = block;
	}

	uint32_t getBlockSize() const {
		return block_size;
	}

	void setBlockSize(uint32_t blockSize) {
		block_size = blockSize;
	}

	uint32_t getMagic() const {
		return magic;
	}

	void setMagic(uint32_t magic) {
		this->magic = magic;
	}

	std::string get_hex() const;

	const std::vector<CBTx>& getVtx() const {
		return vtx;
	}

	void setVtx(const std::vector<CBTx> &vtx) {
		this->vtx = vtx;
	}

	uint64_t getFileByteOffset() const {
		return file_byte_offset;
	}

	void setFileByteOffset(uint64_t fileByteOffset) {
		file_byte_offset = fileByteOffset;
	}

	uint64_t getTableSeq() const {
		return table_seq;
	}

	void setTableSeq(uint64_t tableSeq) {
		table_seq = tableSeq;
	}
};

}

#endif /* MODEL_CBBLOCK_H_ */
