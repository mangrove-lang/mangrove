// SPDX-License-Identifier: BSD-3-Clause
#ifndef PARSER_RECOGNISER_HXX
#define PARSER_RECOGNISER_HXX

#include "../core/utf8/string.hxx"

namespace mangrove::parser
{
	using mangrove::core::utf8::Char;
	using mangrove::core::utf8::String;
	using namespace mangrove::core::utf8::literals;
} // namespace mangrove::parser

namespace mangrove::parser::recognisers
{
	static inline bool isNewLine(const Char &chr) noexcept { return chr == '\r' || chr == '\n'; }
	static inline bool isWhiteSpace(const Char &chr) noexcept { return chr == ' ' || chr == '\t' || isNewLine(chr); }

	static inline bool isAlpha(const Char &chr) noexcept
	{
		return
			(chr >= u8"a"_c && chr <= u8"z"_c) ||
			(chr >= u8"A"_c && chr <= u8"Z"_c) ||
			(chr >= u8"\u00C0"_c && chr <= u8"\u2000"_c) ||
			(chr >= u8"\u2070"_c && chr <= u8"\uD7FF"_c) ||
			(chr >= u8"\uE000"_c && chr <= u8"\uFE4F"_c) ||
			(chr >= u8"\uFE70"_c && chr <= u8"\uFEFF"_c) ||
			(chr >= u8"\U00010000"_c && chr <= u8"\U0002FA1F"_c);
	}

	static inline bool isDigit(const Char &chr) noexcept { return chr >= '0' && chr <= '9'; }
	static inline bool isAlphaNum(const Char &chr) noexcept { return isAlpha(chr) || isDigit(chr); }
	static inline bool isUnderscore(const Char &chr) { return chr == '_'; }
	static inline bool isBeginBin(const Char &chr) noexcept { return chr == 'b' || chr == 'B'; }
	static inline bool isBeginHex(const Char &chr) noexcept { return chr == 'x' || chr == 'X'; }
	static inline bool isBin(const Char &chr) noexcept { return chr == '0' || chr == '1'; }
	static inline bool isOct(const Char &chr) noexcept { return chr >= '0' && chr <= '7'; }

	static inline bool isHex(const Char &chr) noexcept
	{
		return isDigit(chr) ||
			(chr >= 'a' && chr <= 'f') ||
			(chr >= 'A' && chr <= 'F');
	}
} // namespace mangrove::parser::recognisers

#endif /*PARSER_RECOGNISER_HXX*/
