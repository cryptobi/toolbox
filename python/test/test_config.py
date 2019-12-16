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

import unittest
import os
from cryptobi.toolbox.system.CBConfig import CBConfig

class CBConfigTests(unittest.TestCase):

    def setUp(self):
        pass

    def test_env(self):
        e = os.getenv("CRYPTOBI_HOME")
        self.assertTrue(e)
        e = os.getenv("PYTHONPATH")
        self.assertTrue(e)