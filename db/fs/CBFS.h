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
#ifndef DB_FS_CBFS_H_
#define DB_FS_CBFS_H_

#include <memory>
#include "bitcoin/CBBitcoinChainReader.h"

namespace db::fs {

template<class T>
class CBFS {
	static std::shared_ptr<T> _block_reader;
public:
	CBFS();
	~CBFS();
	static std::shared_ptr<T> get_block_reader();
};


} /* namespace db::fs */

#endif /* DB_FS_CBFS_H_ */
