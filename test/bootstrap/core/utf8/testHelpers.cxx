// SPDX-License-Identifier: BSD-3-Clause
#include <crunch++.h>
#include "../../../../src/bootstrap/core/utf8/helpers.hxx"

using namespace std::literals::string_view_literals;
using namespace mangrove::core::utf8::helpers;

constexpr static auto testString1{u8"The quick brown 🦊 jumped over the lazy 🐶"sv};
constexpr static auto testString2{u8"\u005B\u00D8\u04D5\u16A0\u2026\uFFFD\U00010117"sv};

class testHelpers final : public testsuite
{
private:
	void testCountUnits()
	{
		assertEqual(countUnits(testString1), 40);
		assertEqual(countUnits(testString2), 7);
	}

public:
	void registerTests() final
	{
		CRUNCHpp_TEST(testCountUnits)
	}
};

CRUNCHpp_TESTS(testHelpers)
