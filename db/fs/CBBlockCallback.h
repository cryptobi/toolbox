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
#ifndef DB_FS_CBBLOCKCALLBACK_H_
#define DB_FS_CBBLOCKCALLBACK_H_

#include <boost/filesystem.hpp>
#include "db/fs/CBBlockEntry.h"

namespace db::fs {

class CBBlockCallback {
public:
	CBBlockCallback();
	virtual ~CBBlockCallback();

	/*
	 * Called every time before a block file begins to be read.
	 *
	 * */
	virtual void started_file(const boost::filesystem::path &file_path) = 0;

	/*
	 * Called every block read from file.
	 *
	 * */
	virtual void operator()(CBBlockEntry &cbe) = 0;

	/*
	 * Called every time after a block file has been read.
	 *
	 * */
	virtual void done_file(const boost::filesystem::path &file_path) = 0;
};


} //namespace

#endif /* DB_FS_CBBLOCKCALLBACK_H_ */
