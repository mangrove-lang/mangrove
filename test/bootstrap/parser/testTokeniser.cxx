// SPDX-License-Identifier: BSD-3-Clause
#include <crunch++.h>
#include <parser/tokeniser.hxx>

using namespace mangrove::parser;

class testTokeniser : public testsuite
{
private:
	void testBadFileConstruction()
	{
		Tokeniser tokeniser{{}};
		const auto &token{tokeniser.token()};
		assertFalse(token.valid());
	}

public:
	void registerTests() final
	{
		CRUNCHpp_TEST(testBadFileConstruction)
	}
};

CRUNCHpp_TESTS(testTokeniser)
