// SPDX-License-Identifier: BSD-3-Clause
#ifndef PARSER_TOKENISER_HXX
#define PARSER_TOKENISER_HXX

#include <optional>
#include <utility>
#include <substrate/fd>
#include "recogniser.hxx"
#include "types.hxx"

namespace mangrove::parser
{
	inline namespace internal
	{
		using substrate::fd_t;
	} // namespace internal

	struct Tokeniser final
	{
	private:
		fd_t _file;
		Char currentChar{};
		types::Position position{};
		types::Token _token{};

		Char nextChar() noexcept;
		void finaliseToken(std::optional<types::TokenType> type = {}, String &&value = {}) noexcept;
		void readToken() noexcept;
		void readExtendedToken() noexcept;

		void readPartComment() noexcept;
		void readLineComment() noexcept;
		void readEllipsisToken() noexcept;
		void readBinToken() noexcept;
		void readOctToken() noexcept;
		void readHexToken() noexcept;
		void readIntToken() noexcept;
		[[nodiscard]] Char readUnicode(const Char &normalQuote, const Char &escapedQuote) noexcept;
		void readStringToken() noexcept;
		void readCharToken() noexcept;
		void readDivToken() noexcept;
		void readMulToken() noexcept;
		void readAddToken() noexcept;
		void readBooleanToken() noexcept;
		void readBitwiseToken() noexcept;
		void readRelationToken() noexcept;
		void readEqualityToken() noexcept;
		[[nodiscard]] String readAlphaNumToken() noexcept;

	public:
		Tokeniser(fd_t &&file) noexcept;

		[[nodiscard]] auto &token() const noexcept { return _token; }
		types::Token &next() noexcept;
	};
} // namespace mangrove::parser

#endif /*PARSER_TOKENISER_HXX*/
