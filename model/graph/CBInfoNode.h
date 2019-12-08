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
#ifndef MODEL_CBINFONODE_H_
#define MODEL_CBINFONODE_H_

#include <primitives/block.h>
#include <primitives/transaction.h>
#include <string>
#include <cstdint>

class CBInfoNode {

	uint64_t id;
	uint256 block_hash;
	uint256 tx_hash;
	std::string address;
	std::string content;

public:
	CBInfoNode();
	virtual ~CBInfoNode();
};

#endif /* MODEL_CBINFONODE_H_ */
