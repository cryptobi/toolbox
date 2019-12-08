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
#ifndef DB_FS_BITCOIN_CBBITCOINCHAINREADER_H_
#define DB_FS_BITCOIN_CBBITCOINCHAINREADER_H_

#include <iostream>
#include <cstdlib>
#include <boost/asio/thread_pool.hpp>
#include <boost/asio.hpp>
#include <boost/filesystem.hpp>
#include "db/fs/bitcoin/CBBlkReader.h"
#include "db/fs/CBBlockchainReader.h"
#include "db/fs/CBBlockCallback.h"

// void (*callback)(const db::fs::CBBlockEntry&)

namespace db::fs::bitcoin {

template<typename T>
class CBBitcoinChainReader : public CBBlockchainReader<T> {
	boost::filesystem::path skip_until;
public:

	CBBitcoinChainReader();
	~CBBitcoinChainReader();

	void read_blocks_callback(const boost::filesystem::path &file_path, T &callback);
	void read_blocks_callback_threaded(const boost::filesystem::path &file_path, std::vector<T> &callbacks, boost::asio::thread_pool &tpool);

	const boost::filesystem::path& getSkipUntil() const {
		return skip_until;
	}

	void setSkipUntil(const boost::filesystem::path &skipUntil) {
		skip_until = skipUntil;
	}
};

template<typename T>
CBBitcoinChainReader<T>::CBBitcoinChainReader() {

}

template<typename T>
CBBitcoinChainReader<T>::~CBBitcoinChainReader() {

}

template<typename T>
void CBBitcoinChainReader<T>::read_blocks_callback(const boost::filesystem::path &file_path, T &callback) {

	auto files = CBBlkReader::list_block_filenames(file_path);
	bool last_file_reached = false;
	bool do_skip_files = skip_until.generic_string().length() > 0;

	for (const auto &file: files) {

		if (do_skip_files) {
			if (!last_file_reached) {
				if (file == skip_until)	last_file_reached = true;
				else continue;
			}
		} else {
			last_file_reached = true;
		}

		if (last_file_reached) {
			callback.started_file(file);
			CBBlkReader::read_blocks_callback(file, callback);
			callback.done_file(file);
		}
	}

}

template<typename T>
void CBBitcoinChainReader<T>::read_blocks_callback_threaded(const boost::filesystem::path &file_path, std::vector<T> &callbacks, boost::asio::thread_pool &tpool){

	auto files = CBBlkReader::list_block_filenames(file_path);
	int modx = callbacks.size();

	int i = 0;

	bool last_file_reached = false;
	bool do_skip_files = skip_until.generic_string().length() > 0;

	for (const auto &file: files) {

		if (do_skip_files) {
			if (!last_file_reached) {
				if (file == skip_until)
					last_file_reached = true;
				else
					continue;
			}
		} else {
			last_file_reached = true;
		}

		if (last_file_reached) {
			boost::asio::post(tpool, [&, file, i](){
				callbacks[i].started_file(file);
				CBBlkReader::read_blocks_callback(file, callbacks[i]);
				callbacks[i].done_file(file);
			});
			++i %= modx;
		}

	}
}


} /* namespace db */

#endif /* DB_FS_BITCOIN_CBBITCOINCHAINREADER_H_ */
