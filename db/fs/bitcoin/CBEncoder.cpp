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
 * With code from Bitcoin Core. Please see CBEncoder.h header for details.
 *
 */

#include "CBEncoder.h"

namespace db::fs::bitcoin  {

	CBEncoder::CBEncoder() {
	    base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,0);
	    base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,5);
	    base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,128);
	    base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x88, 0xB2, 0x1E};
	    base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x88, 0xAD, 0xE4};
	    chainParams = CreateChainParams("main");
	}

	CBEncoder::~CBEncoder() {
	}

	/*
	 * See Bitcoin Core script/standard.h
	 * */
	txnouttype CBEncoder::Solver(const CScript &scriptPubKey,
			std::vector<std::vector<unsigned char> > &vSolutionsRet) {
	    vSolutionsRet.clear();

	    // Shortcut for pay-to-script-hash, which are more constrained than the other types:
	    // it is always OP_HASH160 20 [20 byte hash] OP_EQUAL
	    if (scriptPubKey.IsPayToScriptHash())
	    {
	        std::vector<unsigned char> hashBytes(scriptPubKey.begin()+2, scriptPubKey.begin()+22);
	        vSolutionsRet.push_back(hashBytes);
	        return TX_SCRIPTHASH;
	    }

	    int witnessversion;
	    std::vector<unsigned char> witnessprogram;
	    if (scriptPubKey.IsWitnessProgram(witnessversion, witnessprogram)) {
	        if (witnessversion == 0 && witnessprogram.size() == WITNESS_V0_KEYHASH_SIZE) {
	            vSolutionsRet.push_back(witnessprogram);
	            return TX_WITNESS_V0_KEYHASH;
	        }
	        if (witnessversion == 0 && witnessprogram.size() == WITNESS_V0_SCRIPTHASH_SIZE) {
	            vSolutionsRet.push_back(witnessprogram);
	            return TX_WITNESS_V0_SCRIPTHASH;
	        }
	        if (witnessversion != 0) {
	            vSolutionsRet.push_back(std::vector<unsigned char>{(unsigned char)witnessversion});
	            vSolutionsRet.push_back(std::move(witnessprogram));
	            return TX_WITNESS_UNKNOWN;
	        }
	        return TX_NONSTANDARD;
	    }

	    // Provably prunable, data-carrying output
	    //
	    // So long as script passes the IsUnspendable() test and all but the first
	    // byte passes the IsPushOnly() test we don't care what exactly is in the
	    // script.
	    if (scriptPubKey.size() >= 1 && scriptPubKey[0] == OP_RETURN && scriptPubKey.IsPushOnly(scriptPubKey.begin()+1)) {
	        return TX_NULL_DATA;
	    }

	    std::vector<unsigned char> data;
	    if (MatchPayToPubkey(scriptPubKey, data)) {
	        vSolutionsRet.push_back(std::move(data));
	        return TX_PUBKEY;
	    }

	    if (MatchPayToPubkeyHash(scriptPubKey, data)) {
	        vSolutionsRet.push_back(std::move(data));
	        return TX_PUBKEYHASH;
	    }

	    unsigned int required;
	    std::vector<std::vector<unsigned char>> keys;
	    if (MatchMultisig(scriptPubKey, required, keys)) {
	        vSolutionsRet.push_back({static_cast<unsigned char>(required)}); // safe as required is in range 1..16
	        vSolutionsRet.insert(vSolutionsRet.end(), keys.begin(), keys.end());
	        vSolutionsRet.push_back({static_cast<unsigned char>(keys.size())}); // safe as size is in range 1..16
	        return TX_MULTISIG;
	    }

	    vSolutionsRet.clear();
	    return TX_NONSTANDARD;
	}

	bool CBEncoder::ExtractDestinations(const CScript &scriptPubKey,
			txnouttype &typeRet, std::vector<CTxDestination> &addressRet,
			int &nRequiredRet) {
		addressRet.clear();
		std::vector<valtype> vSolutions;
		typeRet = this->Solver(scriptPubKey, vSolutions);
		if (typeRet == TX_NONSTANDARD) {
			return false;
		} else if (typeRet == TX_NULL_DATA) {
			// This is data, not addresses
			return false;
		}

		if (typeRet == TX_MULTISIG)
		{
			nRequiredRet = vSolutions.front()[0];
			for (unsigned int i = 1; i < vSolutions.size()-1; i++)
			{
				CPubKey pubKey(vSolutions[i]);
				if (!pubKey.IsValid())
					continue;

				CTxDestination address = PKHash(pubKey);
				addressRet.push_back(address);
			}

			if (addressRet.empty())
				return false;
		}
		else
		{
			nRequiredRet = 1;
			CTxDestination address;
			if (!ExtractDestination(scriptPubKey, address))
			   return false;
			addressRet.push_back(address);
		}

		return true;
	}



	/*
	 * Static subroutines copied verbatim from script/standard.cpp
	 * */

	static bool MatchPayToPubkey(const CScript& script, valtype& pubkey)
	{
	    if (script.size() == CPubKey::PUBLIC_KEY_SIZE + 2 && script[0] == CPubKey::PUBLIC_KEY_SIZE && script.back() == OP_CHECKSIG) {
	        pubkey = valtype(script.begin() + 1, script.begin() + CPubKey::PUBLIC_KEY_SIZE + 1);
	        return CPubKey::ValidSize(pubkey);
	    }
	    if (script.size() == CPubKey::COMPRESSED_PUBLIC_KEY_SIZE + 2 && script[0] == CPubKey::COMPRESSED_PUBLIC_KEY_SIZE && script.back() == OP_CHECKSIG) {
	        pubkey = valtype(script.begin() + 1, script.begin() + CPubKey::COMPRESSED_PUBLIC_KEY_SIZE + 1);
	        return CPubKey::ValidSize(pubkey);
	    }
	    return false;
	}

	static bool MatchPayToPubkeyHash(const CScript& script, valtype& pubkeyhash)
	{
	    if (script.size() == 25 && script[0] == OP_DUP && script[1] == OP_HASH160 && script[2] == 20 && script[23] == OP_EQUALVERIFY && script[24] == OP_CHECKSIG) {
	        pubkeyhash = valtype(script.begin () + 3, script.begin() + 23);
	        return true;
	    }
	    return false;
	}

	/** Test for "small positive integer" script opcodes - OP_1 through OP_16. */
	static constexpr bool IsSmallInteger(opcodetype opcode)
	{
	    return opcode >= OP_1 && opcode <= OP_16;
	}

	static bool MatchMultisig(const CScript& script, unsigned int& required, std::vector<valtype>& pubkeys)
	{
	    opcodetype opcode;
	    valtype data;
	    CScript::const_iterator it = script.begin();
	    if (script.size() < 1 || script.back() != OP_CHECKMULTISIG) return false;

	    if (!script.GetOp(it, opcode, data) || !IsSmallInteger(opcode)) return false;
	    required = CScript::DecodeOP_N(opcode);
	    while (script.GetOp(it, opcode, data) && CPubKey::ValidSize(data)) {
	        pubkeys.emplace_back(std::move(data));
	    }
	    if (!IsSmallInteger(opcode)) return false;
	    unsigned int keys = CScript::DecodeOP_N(opcode);
	    if (pubkeys.size() != keys || keys < required) return false;
	    return (it + 1 == script.end());
	}


	class DestinationEncoder : public boost::static_visitor<std::string>
	{
	private:
	    const CChainParams& m_params;

	public:
	    explicit DestinationEncoder(const CChainParams& params) : m_params(params) {}

	    std::string operator()(const PKHash& id) const
	    {
	        std::vector<unsigned char> data = m_params.Base58Prefix(CChainParams::PUBKEY_ADDRESS);
	        data.insert(data.end(), id.begin(), id.end());
	        return EncodeBase58Check(data);
	    }

	    std::string operator()(const ScriptHash& id) const
	    {
	        std::vector<unsigned char> data = m_params.Base58Prefix(CChainParams::SCRIPT_ADDRESS);
	        data.insert(data.end(), id.begin(), id.end());
	        return EncodeBase58Check(data);
	    }

	    std::string operator()(const WitnessV0KeyHash& id) const
	    {
	        std::vector<unsigned char> data = {0};
	        data.reserve(33);
	        ConvertBits<8, 5, true>([&](unsigned char c) { data.push_back(c); }, id.begin(), id.end());
	        return bech32::Encode(m_params.Bech32HRP(), data);
	    }

	    std::string operator()(const WitnessV0ScriptHash& id) const
	    {
	        std::vector<unsigned char> data = {0};
	        data.reserve(53);
	        ConvertBits<8, 5, true>([&](unsigned char c) { data.push_back(c); }, id.begin(), id.end());
	        return bech32::Encode(m_params.Bech32HRP(), data);
	    }

	    std::string operator()(const WitnessUnknown& id) const
	    {
	        if (id.version < 1 || id.version > 16 || id.length < 2 || id.length > 40) {
	            return {};
	        }
	        std::vector<unsigned char> data = {(unsigned char)id.version};
	        data.reserve(1 + (id.length * 8 + 4) / 5);
	        ConvertBits<8, 5, true>([&](unsigned char c) { data.push_back(c); }, id.program, id.program + id.length);
	        return bech32::Encode(m_params.Bech32HRP(), data);
	    }

	    std::string operator()(const CNoDestination& no) const { return {}; }
	};

	CTxDestination DecodeDestination(const std::string& str, const CChainParams& params)
	{
	    std::vector<unsigned char> data;
	    uint160 hash;
	    if (DecodeBase58Check(str, data)) {
	        // base58-encoded Bitcoin addresses.
	        // Public-key-hash-addresses have version 0 (or 111 testnet).
	        // The data vector contains RIPEMD160(SHA256(pubkey)), where pubkey is the serialized public key.
	        const std::vector<unsigned char>& pubkey_prefix = params.Base58Prefix(CChainParams::PUBKEY_ADDRESS);
	        if (data.size() == hash.size() + pubkey_prefix.size() && std::equal(pubkey_prefix.begin(), pubkey_prefix.end(), data.begin())) {
	            std::copy(data.begin() + pubkey_prefix.size(), data.end(), hash.begin());
	            return PKHash(hash);
	        }
	        // Script-hash-addresses have version 5 (or 196 testnet).
	        // The data vector contains RIPEMD160(SHA256(cscript)), where cscript is the serialized redemption script.
	        const std::vector<unsigned char>& script_prefix = params.Base58Prefix(CChainParams::SCRIPT_ADDRESS);
	        if (data.size() == hash.size() + script_prefix.size() && std::equal(script_prefix.begin(), script_prefix.end(), data.begin())) {
	            std::copy(data.begin() + script_prefix.size(), data.end(), hash.begin());
	            return ScriptHash(hash);
	        }
	    }
	    data.clear();
	    auto bech = bech32::Decode(str);
	    if (bech.second.size() > 0 && bech.first == params.Bech32HRP()) {
	        // Bech32 decoding
	        int version = bech.second[0]; // The first 5 bit symbol is the witness version (0-16)
	        // The rest of the symbols are converted witness program bytes.
	        data.reserve(((bech.second.size() - 1) * 5) / 8);
	        if (ConvertBits<5, 8, false>([&](unsigned char c) { data.push_back(c); }, bech.second.begin() + 1, bech.second.end())) {
	            if (version == 0) {
	                {
	                    WitnessV0KeyHash keyid;
	                    if (data.size() == keyid.size()) {
	                        std::copy(data.begin(), data.end(), keyid.begin());
	                        return keyid;
	                    }
	                }
	                {
	                    WitnessV0ScriptHash scriptid;
	                    if (data.size() == scriptid.size()) {
	                        std::copy(data.begin(), data.end(), scriptid.begin());
	                        return scriptid;
	                    }
	                }
	                return CNoDestination();
	            }
	            if (version > 16 || data.size() < 2 || data.size() > 40) {
	                return CNoDestination();
	            }
	            WitnessUnknown unk;
	            unk.version = version;
	            std::copy(data.begin(), data.end(), unk.program);
	            unk.length = data.size();
	            return unk;
	        }
	    }
	    return CNoDestination();
	}


	std::string CBEncoder::EncodeDestination(const CTxDestination& dest)
	{
	    return boost::apply_visitor(DestinationEncoder(*chainParams), dest);
	}


} // namespace
