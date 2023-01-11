// SPDX-License-Identifier: BSD-3-Clause
#ifndef AST_SYMBOL_TABLE_HXX
#define AST_SYMBOL_TABLE_HXX

#include <cstdint>
#include <map>
#include <memory>
#include <fmt/core.h>
#include "../core/flags.hxx"
#include "../core/utf8/string.hxx"

// This isn't great practice, but we don't get much choice here.
using namespace std::literals::string_view_literals;

namespace mangrove::parser
{
	struct Parser;
} // namespace mangrove::parser

namespace mangrove::ast::symbolTable
{
	using mangrove::core::BitFlags;
	using mangrove::core::utf8::String;
	using mangrove::core::utf8::StringView;
	using mangrove::parser::Parser;

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

namespace mangrove::ast::symbolTable
{
	struct SymbolType final
	{
	private:
		TypeFlags _type{};

	public:
		constexpr SymbolType() noexcept = default;
		constexpr SymbolType(const SymbolTypes symbolType) : _type{symbolType} { }
		constexpr SymbolType(const TypeFlags &symbolType) : _type{symbolType} { }

		// NOLINTNEXTLINE(misc-unconventional-assign-operator)
		constexpr void operator =(const SymbolTypes symbolType) { _type = symbolType; }

		[[nodiscard]] constexpr SymbolType forValue() const noexcept
		{
			if (_type == SymbolTypes::type)
				return *this;
			return {_type.without(SymbolTypes::type)};
		}

		constexpr bool operator ==(const SymbolType &symbolType) const noexcept
			{ return _type == symbolType._type; }
		constexpr bool operator ==(const SymbolTypes &symbolType) const noexcept
			{ return _type == symbolType; }

		[[nodiscard]] constexpr auto isInvalid() const noexcept { return _type == TypeFlags{}; }
		[[nodiscard]] constexpr SymbolType clone() const noexcept { return *this; }

		template<typename... SymbolTypes>
		[[nodiscard]] constexpr auto includes(SymbolTypes ...types) const noexcept
			{ return _type.includes(types...); }
		template<typename... SymbolTypes>
		[[nodiscard]] constexpr SymbolType without(SymbolTypes ...types) const noexcept
			{ return {_type.without(types...)}; }

		[[nodiscard]] std::string toString() const noexcept
		{
			const auto reference{_type.includes(SymbolTypes::reference) ? "reference "sv : ""sv};
			const auto pointer{_type.includes(SymbolTypes::pointer) ? "pointer "sv : ""sv};
			const auto kind{reference.empty() ? pointer : reference};
			const auto signdness{_type.includes(SymbolTypes::unsignedVal) ? "u"sv : ""sv};
			const auto isType{_type != SymbolTypes::type && _type.includes(SymbolTypes::type)};
			auto type{_type.without(SymbolTypes::reference, SymbolTypes::pointer, SymbolTypes::unsignedVal)};
			if (isType)
			{
				type.clear(SymbolTypes::type);
				return fmt::format("type {}'{}{}'"sv, kind, signdness, type);
			}
			return fmt::format("{}{}{}"sv, kind, signdness, type);
		}
	};

	struct Symbol final
	{
	private:
		String _ident;
		SymbolType _type{};
		// std::optional<SymbolStruct> _struct{};

	public:
		Symbol(String &&ident) noexcept : _ident{std::move(ident)} { }
		Symbol(String &&ident, SymbolType &&type) noexcept : _ident{std::move(ident)}, _type{std::move(type)} { }

		bool operator ==(const Symbol &symbol) const noexcept
			{ return _ident == symbol._ident && _type == symbol._type; }

		[[nodiscard]] StringView value() const noexcept { return _ident; }
		void type(const SymbolType &type) noexcept { _type = type; }
		[[nodiscard]] auto type() const noexcept { return _type; }
		[[nodiscard]] auto isType() { return _type.includes(SymbolTypes::type); }
		[[nodiscard]] Symbol clone() const noexcept { return *this; }

		[[nodiscard]] std::string toString() const noexcept
			{ return fmt::format("<Symbol {} -> {}>"sv, _ident, _type.toString()); }
	};

	struct SymbolTable final
	{
	private:
		std::weak_ptr<SymbolTable> _parentTable{};
		std::map<StringView, std::unique_ptr<Symbol>> _table{};

	public:
		SymbolTable(const Parser &parser) noexcept;
		void pop(Parser &parser) const noexcept;

		[[nodiscard]] auto isEmpty() const noexcept { return _table.empty(); }
		[[nodiscard]] auto entryCount() const noexcept { return _table.size(); }

		// This intentionally duplicates `ident` on calling to simplify memory management
		[[nodiscard]] Symbol *add(String ident) noexcept;
		[[nodiscard]] bool insert(const Symbol &symbol) noexcept;

		[[nodiscard]] Symbol *findLocal(const StringView &ident) const noexcept;
		[[nodiscard]] Symbol *find(const StringView &ident) const noexcept;
	};
} // namespace mangrove::ast::symbolTable

#endif /*AST_SYMBOL_TABLE_HXX*/
