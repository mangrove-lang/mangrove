// SPDX-License-Identifier: BSD-3-Clause
#ifndef PARSER_RECOGNISER_HXX
#define PARSER_RECOGNISER_HXX

#include "../core/utf8/string.hxx"

namespace mangrove::parser
{
	using mangrove::core::utf8::Char;
	using mangrove::core::utf8::String;
	using mangrove::core::utf8::StringView;
	using namespace mangrove::core::utf8::literals;
} // namespace mangrove::parser

namespace mangrove::parser::recognisers
{
	static inline bool isNewLine(const Char &chr) noexcept
		{ return chr == '\r'_u8c || chr == '\n'_u8c; }
	static inline bool isWhiteSpace(const Char &chr) noexcept
		{ return chr == ' '_u8c || chr == '\t'_u8c || isNewLine(chr); }

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

	static inline bool isDigit(const Char &chr) noexcept
		{ return chr >= '0'_u8c && chr <= '9'_u8c; }
	static inline bool isAlphaNum(const Char &chr) noexcept
		{ return isAlpha(chr) || isDigit(chr); }
	static inline bool isUnderscore(const Char &chr) noexcept
		{ return chr == '_'_u8c; }
	static inline bool isBeginBin(const Char &chr) noexcept
		{ return chr == 'b'_u8c || chr == 'B'_u8c; }
	static inline bool isBeginHex(const Char &chr) noexcept
		{ return chr == 'x'_u8c || chr == 'X'_u8c; }
	static inline bool isBin(const Char &chr) noexcept
		{ return chr == '0'_u8c || chr == '1'_u8c; }
	static inline bool isOct(const Char &chr) noexcept
		{ return chr >= '0'_u8c && chr <= '7'_u8c; }

	static inline bool isHex(const Char &chr) noexcept
	{
		return isDigit(chr) ||
			(chr >= 'a'_u8c && chr <= 'f'_u8c) ||
			(chr >= 'A'_u8c && chr <= 'F'_u8c);
	}

	static inline bool isDot(const Char &chr) noexcept
		{ return chr == '.'_u8c; }

	static inline bool isNormalAlpha(const Char &chr) noexcept
	{
		return
			chr == u8" "_c || chr == u8"!"_c ||
			(chr >= u8"#"_c && chr <= u8"&"_c) ||
			(chr >= u8"("_c && chr <= u8"["_c) ||
			(chr >= u8"]"_c && chr <= u8"~"_c) ||
			(chr >= u8"\u0080"_c && chr <= u8"\uD7FF"_c) ||
			(chr >= u8"\uE000"_c && chr <= u8"\U0010FFFF"_c);
	}

	static inline bool isSingleQuote(const Char &chr) noexcept
		{ return chr == '\''_u8c; }
	static inline bool isDoubleQuote(const Char &chr) noexcept
		{ return chr == '"'_u8c; }

	static inline bool isTrue(const String &str) noexcept
		{ return str == u8"true"_sv; }
	static inline bool isFalse(const String &str) noexcept
		{ return str == u8"false"_sv; }
	static inline bool isNull(const String &str) noexcept
		{ return str == u8"nullptr"_sv; }

	static inline bool isBeginTmpl(const String &str) noexcept
		{ return str == u8"<"_sv; }
	static inline bool isEndTmpl(const String &str) noexcept
		{ return str == u8">"_sv; }

	static inline bool isEquals(const Char &chr) noexcept
		{ return chr == '='_u8c; }

	static inline bool isEquality(const String &str) noexcept
		{ return str == u8"<"_sv || str == u8"<="_sv || str == u8">"_sv || str == u8">="_sv; }
	static inline bool isRelation(const String &str) noexcept
		{ return isEquality(str) || str == u8"!="_sv || str == u8"=="_sv; }

	static inline bool isNew(const String &str) noexcept
		{ return str == u8"new"_sv; }
	static inline bool isDelete(const String &str) noexcept
		{ return str == u8"delete"_sv; }

	static inline bool isFrom(const String &str) noexcept
		{ return str == u8"from"_sv; }
	static inline bool isImport(const String &str) noexcept
		{ return str == u8"import"_sv; }
	static inline bool isAs(const String &str) noexcept
		{ return str == u8"as"_sv; }

	static inline bool isReturn(const String &str) noexcept
		{ return str == u8"return"_sv; }

	static inline bool isIfStmt(const String &str) noexcept
		{ return str == u8"if"_sv; }
	static inline bool isElifStmt(const String &str) noexcept
		{ return str == u8"elif"_sv; }
	static inline bool isElseStmt(const String &str) noexcept
		{ return str == u8"else"_sv; }

	static inline bool isForStmt(const String &str) noexcept
		{ return str == u8"for"_sv; }
	static inline bool isWhileStmt(const String &str) noexcept
		{ return str == u8"while"_sv; }
	static inline bool isDoStmt(const String &str) noexcept
		{ return str == u8"do"_sv; }

	static inline bool isClass(const String &str) noexcept
		{ return str == u8"class"_sv; }
	static inline bool isEnum(const String &str) noexcept
		{ return str == u8"enum"_sv; }
	static inline bool isFunctionDef(const String &str) noexcept
		{ return str == u8"function"_sv; }
	static inline bool isOperatorDef(const String &str) noexcept
		{ return str == u8"operator"_sv; }

	static inline bool isConst(const String &str) noexcept
		{ return str == u8"const"_sv; }
	static inline bool isStatic(const String &str) noexcept
		{ return str == u8"static"_sv; }
	static inline bool isVolatile(const String &str) noexcept
		{ return str == u8"volatile"_sv; }
	static inline bool isLocationSpec(const String &str) noexcept
		{ return str == u8"eeprom"_s || str == u8"flash"_s || str == u8"rom"_sv; }
	static inline bool isStorageSpec(const String &str) noexcept
		{ return isConst(str) || isVolatile(str) || isStatic(str); }

	static inline bool isNone(const String &str) noexcept
		{ return str == u8"none"_sv; }
	static inline bool isArrow(const String &str) noexcept
		{ return str == u8"->"_sv; }
	static inline bool isVisibility(const String &str) noexcept
		{ return str == u8"public"_s || str == u8"private"_s || str == u8"protected"_sv; }

	static inline bool isUnsafe(const String &str) noexcept
		{ return str == u8"unsafe"_sv; }
} // namespace mangrove::parser::recognisers

#endif /*PARSER_RECOGNISER_HXX*/
