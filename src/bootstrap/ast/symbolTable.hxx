// SPDX-License-Identifier: BSD-3-Clause
#ifndef AST_SYMBOL_TABLE_HXX
#define AST_SYMBOL_TABLE_HXX

#include <cstdint>
#include <fmt/core.h>
#include "../core/flags.hxx"
#include "../core/utf8/string.hxx"

// This isn't great practice, but we don't get much choice here.
using namespace std::literals::string_view_literals;

namespace mangrove::ast::symbolTable
{
	using mangrove::core::BitFlags;
	using mangrove::core::utf8::String;
	using mangrove::core::utf8::StringView;

	enum class SymbolTypes : uint16_t
	{
		// Flags for integral types
		integerVal = 0U,
		// Integer kind flags
		signedVal = 0U, // Alias integerVal for signed
		unsignedVal = 1U,
		// Integer width flags
		int8Bit = 0U, // Alias integerVal for 8-bit
		int16Bit = 2U,
		int32Bit = 3U,
		int64Bit = 4U,
		// Flags for other core types
		character = 8U,
		list = 9U,
		// strings are character | list
		structVal = 10U,
		// dictionaries are structVal | list
		array = 11U,
		// sets are structVal | array
		boolVal = 12U,
		function = 13U,
		reference = 14U,
		pointer = 15U,
		// This is for template type/value packs
		pack = 16U,
		// Placeholder for a type to be determined by the RHS of an assignment
		autoVal = 17U,
		none = 18U,
		type = 19U
	};

	using TypeFlags = BitFlags<uint32_t, SymbolTypes>;
} // namespace mangrove::ast::symbolTable

template<> struct fmt::formatter<mangrove::ast::symbolTable::TypeFlags>
{
	using SymbolTypes = mangrove::ast::symbolTable::SymbolTypes;
	using TypeFlags = mangrove::ast::symbolTable::TypeFlags;

	constexpr auto flagsToValue(const TypeFlags &flags) const noexcept
	{
		switch (flags.toRaw())
		{
			case TypeFlags{SymbolTypes::int8Bit}.toRaw():
				return "Int8"sv;
			case TypeFlags{SymbolTypes::int16Bit}.toRaw():
				return "Int16"sv;
			case TypeFlags{SymbolTypes::int32Bit}.toRaw():
				return "Int32"sv;
			case TypeFlags{SymbolTypes::int64Bit}.toRaw():
				return "Int64"sv;
			case TypeFlags{SymbolTypes::character}.toRaw():
				return "Char"sv;
			case TypeFlags{SymbolTypes::character, SymbolTypes::list}.toRaw():
				return "String"sv;
			case TypeFlags{SymbolTypes::boolVal}.toRaw():
				return "Bool"sv;
			case TypeFlags{SymbolTypes::structVal}.toRaw():
				return "class"sv;
			case TypeFlags{SymbolTypes::list}.toRaw():
				return "List"sv;
			case TypeFlags{SymbolTypes::array}.toRaw():
				return "Array"sv;
			case TypeFlags{SymbolTypes::structVal, SymbolTypes::list}.toRaw():
				return "Dict"sv;
			case TypeFlags{SymbolTypes::structVal, SymbolTypes::array}.toRaw():
				return "Set"sv;
			case TypeFlags{SymbolTypes::autoVal}.toRaw():
				return "auto"sv;
			case TypeFlags{SymbolTypes::none}.toRaw():
				return "none"sv;
			case TypeFlags{SymbolTypes::type}.toRaw():
				return "type"sv;
			case TypeFlags{SymbolTypes::function}.toRaw():
				return "function"sv;
			default:
				return "invalid"sv;
		}
	}

	constexpr auto parse(format_parse_context &ctx)
	{
		if (ctx.begin() != ctx.end())
			throw format_error{"invalid format"};
		return ctx.end();
	}

	template<typename FormatContext> auto format(const TypeFlags &flags, FormatContext &ctx) const
		{ return fmt::format_to(ctx.out(), "{}"sv, flagsToValue(flags)); }
};

#endif /*AST_SYMBOL_TABLE_HXX*/
