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
#include "CBDAO.h"

namespace db::dao {

std::vector<CBDAODriver *> CBDAO::_drivers;
std::mutex CBDAO::_mutex;
int CBDAO::_driver_counter = 0;
int CBDAO::pool_size;
bool CBDAO::first_run = true;

CBDAODriver *CBDAO::get_DAO() {

	std::scoped_lock<std::mutex> l{_mutex};

	if (first_run) {
		init_dao();
		first_run = false;
	}

	auto ret = _drivers[_driver_counter];
	++_driver_counter %= pool_size;

	return ret;
}

void CBDAO::init_dao() {
	_driver_counter = 0;
	pool_size = std::thread::hardware_concurrency();
	_drivers.reserve(pool_size);
	for (int i=0;i<pool_size;i++) {
		_drivers[i] = new mysql::CBMySQL();
	}
}


} /* namespace db */

