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
 * A rewrite of bool ExtractDestinations(const CScript& scriptPubKey, txnouttype& typeRet, std::vector<CTxDestination>& addressRet, int& nRequiredRet)
 * from Bitcoin Core (script/standard.cpp).
 *
 * This version doesn't require global state or a running node to work.
 * (E.g. The "main" network parameters are hardcoded here.)
 *
 * The below copyright notice for Bitcoin Core code included in this implementation (script/standard.{cpp,h}):
 *
 * // Copyright (c) 2009-2010 Satoshi Nakamoto
 * // Copyright (c) 2009-2018 The Bitcoin Core developers
 * // Distributed under the MIT software license, see the accompanying
 * // file COPYING or http://www.opensource.org/licenses/mit-license.php.
 *
 */

#ifndef CBENCODER_H_
#define CBENCODER_H_

#include <vector>
#include <string>
#include <pubkey.h>
#include <script/script.h>
#include <script/standard.h>
#include <boost/variant.hpp>
#include <boost/variant/apply_visitor.hpp>
#include <boost/variant/static_visitor.hpp>
#include <key_io.h>
#include <memory>
#include <base58.h>
#include <util/strencodings.h>
#include <bech32.h>

typedef std::vector<unsigned char> valtype;

namespace db::fs::bitcoin {


	/*
	 * Static subroutines copied verbatim from script/standard.cpp
	 * */
	static bool MatchPayToPubkeyHash(const CScript& script, valtype& pubkeyhash);
	static bool MatchPayToPubkey(const CScript& script, valtype& pubkey);
	static constexpr bool IsSmallInteger(opcodetype opcode);
	static bool MatchMultisig(const CScript& script, unsigned int& required, std::vector<valtype>& pubkeys);


	class CBEncoder {

	public:

	    enum Base58Type {
	        PUBKEY_ADDRESS,
	        SCRIPT_ADDRESS,
	        SECRET_KEY,
	        EXT_PUBLIC_KEY,
	        EXT_SECRET_KEY,
	        MAX_BASE58_TYPES
	    };


		CBEncoder();
		virtual ~CBEncoder();
		txnouttype Solver(const CScript& scriptPubKey, std::vector<std::vector<unsigned char>>& vSolutionsRet);
		bool ExtractDestinations(const CScript& scriptPubKey, txnouttype& typeRet, std::vector<CTxDestination>& addressRet, int& nRequiredRet);
		std::string EncodeDestination(const CTxDestination& dest);

	private:

		std::vector<unsigned char> base58Prefixes[MAX_BASE58_TYPES];
		std::unique_ptr<const CChainParams> chainParams;

	};

}

#endif /* CBENCODER_H_ */
