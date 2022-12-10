// SPDX-License-Identifier: BSD-3-Clause
#ifndef PARSER_TYPES_HXX
#define PARSER_TYPES_HXX

#include <cstdint>
#include <cstddef>
#include <utility>
#include "../core/utf8/string.hxx"

namespace mangrove::parser
{
	using mangrove::core::utf8::String;
	using mangrove::core::utf8::StringView;
} // namespace mangrove::parser

namespace mangrove::parser::types
{
	enum class TokenType : uint16_t
	{
		invalid,
		eof,
		whitespace,
		comment,
		newline,
		dot,
		ellipsis,
		semi,
		ident,
		leftParen,
		rightParen,
		leftBrace,
		rightBrace,
		leftSquare,
		rightSquare,
		comma,
		colon,
		binLit,
		octLit,
		hexLit,
		intLit,
		stringLit,
		charLit,
		boolLit,
		float32Lit,
		float64Lit,
		nullptrLit,
		invert,
		incOp,
		mulOp,
		addOp,
		shiftOp,
		bitOp,
		relOp,
		equOp,
		logicOp,

		locationSpec,
		storageSpec,
		type,
		assignOp,

		fromStmt,
		importStmt,
		asStmt,
		newStmt,
		deleteStmt,
		returnStmt,
		ifStmt,
		elifStmt,
		elseStmt,
		forStmt,
		whileStmt,
		doStmt,

		noneType,
		arrow,
		classDef,
		enumDef,
		functionDef,
		operatorDef,
		decorator,
		visibility,
		unsafe
	};

	struct Position final
	{
		size_t line{};
		size_t character{};
	};

	struct FileSegment final
	{
		Position begin{SIZE_MAX, SIZE_MAX};
		Position end{SIZE_MAX, SIZE_MAX};
	};

	struct Token final
	{
	private:
		TokenType _type{TokenType::invalid};
		String _value{};
		FileSegment _location{};

	public:
		Token() noexcept = default;
		Token(const Token &token) noexcept;
		Token(Token &&token) noexcept : Token{} { swap(token); }

		[[nodiscard]] auto type() const noexcept { return _type; }
		[[nodiscard]] StringView value() const noexcept { return _value; }
		void value(String &&value) noexcept { _value = std::move(value); }
		void value(const StringView &value) noexcept { _value = value; }
		[[nodiscard]] auto location() const noexcept { return _location; }
		[[nodiscard]] bool valid() const noexcept { return _type != TokenType::invalid; }

		void set(const TokenType type, String &&value = String{})
		{
			_type = type;
			_value = std::move(value);
		}

		void reset()
		{
			_type = TokenType::invalid;
			_value = {};
			_location.begin = _location.end;
		}

		void beginsAt(const Position position)
			{ _location.begin = position; }
		void endsAt(const Position position)
			{ _location.end = position; }

		void swap(Token &token) noexcept;
	};
} // namespace mangrove::parser::types

#endif /*PARSER_TYPES_HXX*/
