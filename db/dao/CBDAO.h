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
#ifndef DB_DAO_CBDAO_H_
#define DB_DAO_CBDAO_H_

#include <memory>
#include <mutex>
#include <vector>
#include "db/dao/mysql/CBMySQL.h"

namespace db::dao {

class CBDAO {

	static std::mutex _mutex;
	static int _driver_counter;
	static std::vector<CBDAODriver *> _drivers;
	static int pool_size;
	static void init_dao();
	static bool first_run;

public:

	static CBDAODriver *get_DAO();

};

}

#endif /* DB_DAO_CBDAO_H_ */
