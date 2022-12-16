// SPDX-License-Identifier: BSD-3-Clause
#include <filesystem>
#include <substrate/console>
#include <crunch++.h>
#include "../../../src/bootstrap/parser/tokeniser.hxx"

using std::filesystem::path;
using std::filesystem::current_path;
using std::filesystem::canonical;
using namespace std::literals::string_view_literals;
using substrate::fd_t;
using substrate::console;
using namespace mangrove::core::utf8::literals;
using mangrove::core::utf8::StringView;
using mangrove::parser::Tokeniser;
using mangrove::parser::types::TokenType;

class testTokeniser final : public testsuite
{
private:
	path casesPath{canonical(current_path() / ".." / ".." / "cases" / "tokenisation")};

	Tokeniser tokeniserFor(const std::string_view &file)
	{
		const auto fileName{casesPath / file};
		return {fd_t{fileName.c_str(), O_RDONLY | O_NOCTTY}};
	}

	void readInvalid(Tokeniser &tokeniser)
	{
		const auto &token{tokeniser.next()};
		assertFalse(token.valid());
		assertEqual(token.type(), TokenType::invalid);
		assertTrue(token.value().isEmpty());
	}

	void readNewline(Tokeniser &tokeniser)
	{
		const auto &token{tokeniser.next()};
		assertTrue(token.valid());
		assertEqual(token.type(), TokenType::newline);
		assertTrue(token.value().isEmpty());
	}

	void readValue(Tokeniser &tokeniser, const TokenType expectedType, const StringView &expectedValue)
	{
		const auto &token{tokeniser.next()};
		assertTrue(token.valid());
		assertEqual(token.type(), expectedType);
		assertTrue(token.value() == expectedValue);
	}

	void readEmptyValue(Tokeniser &tokeniser, const TokenType expectedType)
	{
		const auto &token{tokeniser.next()};
		assertTrue(token.valid());
		assertEqual(token.type(), expectedType);
		assertTrue(token.value().isEmpty());
	}

	void readEOF(Tokeniser &tokeniser)
	{
		const auto &token{tokeniser.next()};
		assertTrue(token.valid());
		assertEqual(token.type(), TokenType::eof);
		assertTrue(token.value().isEmpty());
	}

	void testBadFileConstruction()
	{
		Tokeniser tokeniser{fd_t{}};
		const auto &token{tokeniser.token()};
		assertFalse(token.valid());
		assertTrue(&token == &tokeniser.next());
		assertFalse(token.valid());
	}

	void testIntegralLiterals()
	{
		auto tokeniser{tokeniserFor("integralLiterals.case"sv)};
		// Check that the tokeniser start off in an invalid state
		const auto &token{tokeniser.token()};
		assertFalse(token.valid());

		// It is assumed after each test value that a single Linux-style new line follows
		// Consume the first token from the input and start testing tokenisation
		console.info("Checking tokenisation of '0'"sv);
		readValue(tokeniser, TokenType::intLit, u8"0"_sv);
		readNewline(tokeniser);
		console.info("Checking tokenisation of '07'"sv);
		readValue(tokeniser, TokenType::intLit, u8"07"_sv);
		readNewline(tokeniser);
		console.info("Checking tokenisation of '08'"sv);
		readValue(tokeniser, TokenType::intLit, u8"08"_sv);
		readNewline(tokeniser);
		console.info("Checking tokenisation of '0b1001'"sv);
		readValue(tokeniser, TokenType::binLit, u8"1001"_sv);
		readNewline(tokeniser);
		console.info("Checking tokenisation of '0b'"sv);
		readInvalid(tokeniser);
		readNewline(tokeniser);
		console.info("Checking tokenisation of '0c11'"sv);
		readValue(tokeniser, TokenType::octLit, u8"11"_sv);
		readNewline(tokeniser);
		console.info("Checking tokenisation of '0c'"sv);
		readInvalid(tokeniser);
		readNewline(tokeniser);
		console.info("Checking tokenisation of '0x95'"sv);
		readValue(tokeniser, TokenType::hexLit, u8"95"_sv);
		readNewline(tokeniser);
		console.info("Checking tokenisation of '0x'"sv);
		readInvalid(tokeniser);
		readNewline(tokeniser);
		console.info("Checking tokenisation of '100'"sv);
		readValue(tokeniser, TokenType::intLit, u8"100"_sv);
		readNewline(tokeniser);
		console.info("Checking tokenisation of '6'"sv);
		readValue(tokeniser, TokenType::intLit, u8"6"_sv);
		readNewline(tokeniser);
		console.info("Checking tokenisation of '0a'"sv);
		readValue(tokeniser, TokenType::intLit, u8"0"_sv);
		readValue(tokeniser, TokenType::ident, u8"a"_sv);
		readNewline(tokeniser);
		// Finally, consume one last token and make sure it's the EOF token
		readEOF(tokeniser);
	}

	void testStringLiterals()
	{
		auto tokeniser{tokeniserFor("stringLiterals.case"sv)};
		// Check that the tokeniser start off in an invalid state
		const auto &token{tokeniser.token()};
		assertFalse(token.valid());

		// It is assumed after each test value that a single Linux-style new line follows
		// Consume the first token from the input and start testing tokenisation
		console.info("Checking tokenisation of \"\""sv);
		readValue(tokeniser, TokenType::stringLit, u8""_sv);
		readNewline(tokeniser);
		console.info("Checking tokenisation of \"The quick brown fox jumped over the lazy dog\""sv);
		readValue(tokeniser, TokenType::stringLit, u8"The quick brown fox jumped over the lazy dog"_sv);
		readNewline(tokeniser);
		console.info("Checking tokenisation of \"\ufffd\""sv);
		readValue(tokeniser, TokenType::stringLit, u8"\ufffd"_sv);
		readNewline(tokeniser);
		console.info("Checking tokenisation of \"⛏\""sv);
		readValue(tokeniser, TokenType::stringLit, u8"⛏"_sv);
		readNewline(tokeniser);
		console.info("Checking tokenisation of \"🥭💖\""sv);
		readValue(tokeniser, TokenType::stringLit, u8"🥭💖"_sv);
		readNewline(tokeniser);
		console.info("Checking tokenisation of \"🎉 unicode literals 🎊\""sv);
		readValue(tokeniser, TokenType::stringLit, u8"🎉 unicode literals 🎊"_sv);
		readNewline(tokeniser);
		// console.info("Checking tokenisation of ''"sv);
		// readValue(tokeniser, TokenType::charLit, u8""_sv);
		// readNewline(tokeniser);
		// console.info("Checking tokenisation of '🍑'"sv);
		// readValue(tokeniser, TokenType::charLit, u8"🍑"_sv);
		// readNewline(tokeniser);
		// Finally, consume one last token and make sure it's the EOF token
		//readEOF(tokeniser);
	}

public:
	void registerTests() final
	{
		console = {stdout, stderr};
		CRUNCHpp_TEST(testBadFileConstruction)
		CRUNCHpp_TEST(testIntegralLiterals)
		CRUNCHpp_TEST(testStringLiterals)
	}
};

CRUNCHpp_TESTS(testTokeniser)
