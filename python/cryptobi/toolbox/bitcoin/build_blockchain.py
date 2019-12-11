"""

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

"""

from cryptobi.toolbox.system.CBConfig import CBConfig
from cryptobi.model.graph.CBAGNode import CBAGNode
from cryptobi.model.blockchains.CBBlock import CBBlock
from cryptobi.db.dao.CBDAO import CBDAO
import sys

config = CBConfig.get_config()
dao = CBDAO().get_DAO()
genesis = CBBlock.genesis()

