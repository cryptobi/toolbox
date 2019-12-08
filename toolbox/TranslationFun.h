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

// Bitcoin Core requires G_TRANSLATION_FUN
// Just a stub to satisfy requirement.

#include <string>
#include <functional>

extern const inline std::function<std::string(const char*)> G_TRANSLATION_FUN = [](const char* psz) {
    return std::string{psz};
};
