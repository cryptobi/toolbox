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
#ifndef DB_FS_CBBLOCKCHAINREADER_H_
#define DB_FS_CBBLOCKCHAINREADER_H_

#include <string>
#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/asio/thread_pool.hpp>
#include "CBBlockEntry.h"
#include "db/fs/CBBlockCallback.h"

namespace db::fs {

	class DummyCallback : public db::fs::CBBlockCallback {
	public:

		void operator()(db::fs::CBBlockEntry &cbe) override {
		}

		void started_file(const boost::filesystem::path &file_path) override {
		}

		void done_file(const boost::filesystem::path &file_path) {
		}

	};

// Interface for block readers.
template<typename T = DummyCallback>
class CBBlockchainReader {
public:
	virtual ~CBBlockchainReader();
	// read directory of block files
	void read_blocks_callback(const boost::filesystem::path &file_path, T &callback);
	void read_blocks_callback_threaded(const boost::filesystem::path &file_path, std::vector<T> &callbacks, boost::asio::thread_pool &tpool);
};

template<typename T>
CBBlockchainReader<T>::~CBBlockchainReader() {
}

template<typename T>
void CBBlockchainReader<T>::read_blocks_callback_threaded(
		const boost::filesystem::path &file_path, std::vector<T> &callbacks,
		boost::asio::thread_pool &tpool) {
}

template<typename T>
void CBBlockchainReader<T>::read_blocks_callback(
		const boost::filesystem::path &file_path, T &callback) {
}

} /* namespace db::fs */

#endif /* DB_FS_CBBLOCKCHAINREADER_H_ */
