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
#include "CBFS.h"

namespace db::fs {

	template<class T>
	std::shared_ptr<T> CBFS<T>::_block_reader = std::make_shared<T>();

	template<class T>
	CBFS<T>::CBFS() {}

	template<class T>
	CBFS<T>::~CBFS() {}

	template<class T>
	std::shared_ptr<T> CBFS<T>::get_block_reader() {
		if (_block_reader == nullptr) {
			_block_reader = std::make_shared<T>();
		}
		return _block_reader;
	}


} /* namespace db */
