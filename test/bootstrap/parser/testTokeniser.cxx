// SPDX-License-Identifier: BSD-3-Clause
#include <filesystem>
#include <substrate/console>
#include <crunch++.h>
#include "../../../src/bootstrap/parser/tokeniser.hxx"

using std::filesystem::path;
using std::filesystem::current_path;
using std::filesystem::canonical;
using substrate::fd_t;
using substrate::console;
using mangrove::parser::Tokeniser;
using mangrove::parser::types::TokenType;

class testTokeniser final : public testsuite
{
private:
	path casesPath{canonical(current_path() / ".." / ".." / "cases" / "tokenisation")};

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
		const auto fileName{casesPath / "integralLiterals.case"};
		Tokeniser tokeniser{fd_t{fileName.c_str(), O_RDONLY | O_NOCTTY}};

		// Check that the tokeniser start off in an invalid state
		const auto &token{tokeniser.token()};
		assertFalse(token.valid());

		// Consume the first token from the input and ensure it's valid
		tokeniser.next();
		assertTrue(token.valid());
		// assertEqual(token.type(), TokenType::intLit);
	}

public:
	void registerTests() final
	{
		console = {stdout, stderr};
		CRUNCHpp_TEST(testBadFileConstruction)
		CRUNCHpp_TEST(testIntegralLiterals)
	}
};

CRUNCHpp_TESTS(testTokeniser)
