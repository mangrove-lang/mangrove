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

	void readWhitespace(Tokeniser &tokeniser)
	{
		const auto &token{tokeniser.next()};
		assertTrue(token.valid());
		assertEqual(token.type(), TokenType::whitespace);
		if (token.value() == u8" "sv || token.value() == u8"\t"sv)
			assertFalse(token.value().isEmpty());
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

	void readAssignment(Tokeniser &tokeniser, const StringView &identValue, const StringView &assignOpValue,
		const StringView &literalValue)
	{
		readValue(tokeniser, TokenType::ident, identValue);
		readWhitespace(tokeniser);
		readValue(tokeniser, TokenType::assignOp, assignOpValue);
		readWhitespace(tokeniser);
		readValue(tokeniser, TokenType::intLit, literalValue);
		readNewline(tokeniser);
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
		console.info("Checking tokenisation of ''"sv);
		readInvalid(tokeniser);
		readNewline(tokeniser);
		console.info("Checking tokenisation of '🍑'"sv);
		readValue(tokeniser, TokenType::charLit, u8"🍑"_sv);
		readNewline(tokeniser);
		console.info("Checking tokenisation of '⛏'"sv);
		readValue(tokeniser, TokenType::charLit, u8"⛏"_sv);
		readNewline(tokeniser);
		console.info("Checking tokenisation of \"'\""sv);
		readValue(tokeniser, TokenType::stringLit, u8"'"_sv);
		readNewline(tokeniser);
		console.info("Checking tokenisation of \"\\\"\""sv);
		readValue(tokeniser, TokenType::stringLit, u8"\""_sv);
		readNewline(tokeniser);
		console.info("Checking tokenisation of '\\''"sv);
		readValue(tokeniser, TokenType::charLit, u8"'"_sv);
		readNewline(tokeniser);
		console.info("Checking tokenisation of '\"'"sv);
		readValue(tokeniser, TokenType::charLit, u8"\""_sv);
		readNewline(tokeniser);
		console.info("Checking tokenisation of '\\\\'"sv);
		readValue(tokeniser, TokenType::charLit, u8"\\"_sv);
		readNewline(tokeniser);
		console.info("Checking tokenisation of '\\r'"sv);
		readValue(tokeniser, TokenType::charLit, u8"\r"_sv);
		readNewline(tokeniser);
		console.info("Checking tokenisation of '\\n'"sv);
		readValue(tokeniser, TokenType::charLit, u8"\n"_sv);
		readNewline(tokeniser);
		console.info("Checking tokenisation of '\\b'"sv);
		readValue(tokeniser, TokenType::charLit, u8"\b"_sv);
		readNewline(tokeniser);
		console.info("Checking tokenisation of '\\v'"sv);
		readValue(tokeniser, TokenType::charLit, u8"\v"_sv);
		readNewline(tokeniser);
		console.info("Checking tokenisation of '\\t'"sv);
		readValue(tokeniser, TokenType::charLit, u8"\t"_sv);
		readNewline(tokeniser);
		console.info("Checking tokenisation of '\\a'"sv);
		readValue(tokeniser, TokenType::charLit, u8"\a"_sv);
		readNewline(tokeniser);
		console.info("Checking tokenisation of '\\f'"sv);
		readValue(tokeniser, TokenType::charLit, u8"\f"_sv);
		readNewline(tokeniser);
		// Finally, consume one last token and make sure it's the EOF token
		readEOF(tokeniser);
	}

	void testAssignments()
	{
		auto tokeniser{tokeniserFor("assignments.case"sv)};
		// Check that the tokeniser start off in an invalid state
		const auto &token{tokeniser.token()};
		assertFalse(token.valid());

		// It is assumed after each test value that a single Linux-style new line follows
		// Consume the first token from the input and start testing tokenisation
		console.info("Checking tokenisation of 'a = 1'"sv);
		readAssignment(tokeniser, u8"a"_sv, u8"="_sv, u8"1"_sv);
		console.info("Checking tokenisation of 'b += 2'"sv);
		readAssignment(tokeniser, u8"b"_sv, u8"+="_sv, u8"2"_sv);
		console.info("Checking tokenisation of 'c -= 3'"sv);
		readAssignment(tokeniser, u8"c"_sv, u8"-="_sv, u8"3"_sv);
		console.info("Checking tokenisation of 'd *= 4'"sv);
		readAssignment(tokeniser, u8"d"_sv, u8"*="_sv, u8"4"_sv);
		console.info("Checking tokenisation of 'e /= 5'"sv);
		readAssignment(tokeniser, u8"e"_sv, u8"/="_sv, u8"5"_sv);
		console.info("Checking tokenisation of 'f %= 6'"sv);
		readAssignment(tokeniser, u8"f"_sv, u8"%="_sv, u8"6"_sv);
		console.info("Checking tokenisation of 'g &= 7'"sv);
		readAssignment(tokeniser, u8"g"_sv, u8"&="_sv, u8"7"_sv);
		console.info("Checking tokenisation of 'h |= 8'"sv);
		readAssignment(tokeniser, u8"h"_sv, u8"|="_sv, u8"8"_sv);
		console.info("Checking tokenisation of 'i ^= 9'"sv);
		readAssignment(tokeniser, u8"i"_sv, u8"^="_sv, u8"9"_sv);
		console.info("Checking tokenisation of 'j >>= 10'"sv);
		readAssignment(tokeniser, u8"j"_sv, u8"<<="_sv, u8"10"_sv);
		console.info("Checking tokenisation of 'k <<= 11'"sv);
		readAssignment(tokeniser, u8"k"_sv, u8">>="_sv, u8"11"_sv);
		// Finally, consume one last token and make sure it's the EOF token
		readEOF(tokeniser);
		readEOF(tokeniser);
	}

	void testKeywords()
	{
		auto tokeniser{tokeniserFor("keywords.case"sv)};
		// Check that the tokeniser start off in an invalid state
		const auto &token{tokeniser.token()};
		assertFalse(token.valid());

		// It is assumed after each test value that a single Linux-style new line follows
		// Consume the first token from the input and start testing tokenisation
		console.info("Checking tokenisation of 'true'"sv);
		readValue(tokeniser, TokenType::boolLit, u8"true"_sv);
		readNewline(tokeniser);
		console.info("Checking tokenisation of 'false'"sv);
		readValue(tokeniser, TokenType::boolLit, u8"false"_sv);
		readNewline(tokeniser);
		console.info("Checking tokenisation of 'nullptr'"sv);
		readValue(tokeniser, TokenType::nullptrLit, u8"nullptr"_sv);
		readNewline(tokeniser);
		console.info("Checking tokenisation of 'and'"sv);
		readValue(tokeniser, TokenType::logicOp, u8"&"_sv);
		readNewline(tokeniser);
		console.info("Checking tokenisation of 'or'"sv);
		readValue(tokeniser, TokenType::logicOp, u8"|"_sv);
		readNewline(tokeniser);
		console.info("Checking tokenisation of 'not'"sv);
		readValue(tokeniser, TokenType::logicOp, u8"!"_sv);
		readNewline(tokeniser);
		console.info("Checking tokenisation of 'eeprom'"sv);
		readValue(tokeniser, TokenType::locationSpec, u8"eeprom"_sv);
		readNewline(tokeniser);
		console.info("Checking tokenisation of 'flash'"sv);
		readValue(tokeniser, TokenType::locationSpec, u8"flash"_sv);
		readNewline(tokeniser);
		console.info("Checking tokenisation of 'rom'"sv);
		readValue(tokeniser, TokenType::locationSpec, u8"rom"_sv);
		readNewline(tokeniser);
		console.info("Checking tokenisation of 'const'"sv);
		readValue(tokeniser, TokenType::storageSpec, u8"const"_sv);
		readNewline(tokeniser);
		console.info("Checking tokenisation of 'static'"sv);
		readValue(tokeniser, TokenType::storageSpec, u8"static"_sv);
		readNewline(tokeniser);
		console.info("Checking tokenisation of 'volatile'"sv);
		readValue(tokeniser, TokenType::storageSpec, u8"volatile"_sv);
		readNewline(tokeniser);
		console.info("Checking tokenisation of 'new'"sv);
		readValue(tokeniser, TokenType::newStmt, u8"new"_sv);
		readNewline(tokeniser);
		console.info("Checking tokenisation of 'delete'"sv);
		readValue(tokeniser, TokenType::deleteStmt, u8"delete"_sv);
		readNewline(tokeniser);
		console.info("Checking tokenisation of 'from'"sv);
		readValue(tokeniser, TokenType::fromStmt, u8"from"_sv);
		readNewline(tokeniser);
		console.info("Checking tokenisation of 'import'"sv);
		readValue(tokeniser, TokenType::importStmt, u8"import"_sv);
		readNewline(tokeniser);
		console.info("Checking tokenisation of 'as'"sv);
		readValue(tokeniser, TokenType::asStmt, u8"as"_sv);
		readNewline(tokeniser);
		console.info("Checking tokenisation of 'return'"sv);
		readValue(tokeniser, TokenType::returnStmt, u8"return"_sv);
		readNewline(tokeniser);
		console.info("Checking tokenisation of 'if'"sv);
		readValue(tokeniser, TokenType::ifStmt, u8"if"_sv);
		readNewline(tokeniser);
		console.info("Checking tokenisation of 'elif'"sv);
		readValue(tokeniser, TokenType::elifStmt, u8"elif"_sv);
		readNewline(tokeniser);
		console.info("Checking tokenisation of 'else'"sv);
		readValue(tokeniser, TokenType::elseStmt, u8"else"_sv);
		readNewline(tokeniser);
		console.info("Checking tokenisation of 'for'"sv);
		readValue(tokeniser, TokenType::forStmt, u8"for"_sv);
		readNewline(tokeniser);
		console.info("Checking tokenisation of 'while'"sv);
		readValue(tokeniser, TokenType::whileStmt, u8"while"_sv);
		readNewline(tokeniser);
		console.info("Checking tokenisation of 'do'"sv);
		readValue(tokeniser, TokenType::doStmt, u8"do"_sv);
		readNewline(tokeniser);
		console.info("Checking tokenisation of 'none'"sv);
		readValue(tokeniser, TokenType::noneType, u8"none"_sv);
		readNewline(tokeniser);
		console.info("Checking tokenisation of 'class'"sv);
		readValue(tokeniser, TokenType::classDef, u8"class"_sv);
		readNewline(tokeniser);
		console.info("Checking tokenisation of 'enum'"sv);
		readValue(tokeniser, TokenType::enumDef, u8"enum"_sv);
		readNewline(tokeniser);
		console.info("Checking tokenisation of 'function'"sv);
		readValue(tokeniser, TokenType::functionDef, u8"function"_sv);
		readNewline(tokeniser);
		console.info("Checking tokenisation of 'operator'"sv);
		readValue(tokeniser, TokenType::operatorDef, u8"operator"_sv);
		readNewline(tokeniser);
		console.info("Checking tokenisation of 'public'"sv);
		readValue(tokeniser, TokenType::visibility, u8"public"_sv);
		readNewline(tokeniser);
		console.info("Checking tokenisation of 'protected'"sv);
		readValue(tokeniser, TokenType::visibility, u8"protected"_sv);
		readNewline(tokeniser);
		console.info("Checking tokenisation of 'private'"sv);
		readValue(tokeniser, TokenType::visibility, u8"private"_sv);
		readNewline(tokeniser);
		console.info("Checking tokenisation of 'unsafe'"sv);
		readValue(tokeniser, TokenType::unsafe, u8"unsafe"_sv);
		readNewline(tokeniser);
		// Finally, consume one last token and make sure it's the EOF token
		readEOF(tokeniser);
	}

	void testPunctuation()
	{
		auto tokeniser{tokeniserFor("punctuation.case"sv)};
		// Check that the tokeniser start off in an invalid state
		const auto &token{tokeniser.token()};
		assertFalse(token.valid());

		// It is assumed after each test value that a single Linux-style new line follows
		// Consume the first token from the input and start testing tokenisation
		console.info("Checking tokenisation of '.'"sv);
		readEmptyValue(tokeniser, TokenType::dot);
		readNewline(tokeniser);
		console.info("Checking tokenisation of '..'"sv);
		readEmptyValue(tokeniser, TokenType::dot);
		readEmptyValue(tokeniser, TokenType::dot);
		readNewline(tokeniser);
		console.info("Checking tokenisation of '...'"sv);
		readEmptyValue(tokeniser, TokenType::ellipsis);
		readNewline(tokeniser);
		console.info("Checking tokenisation of '...'"sv);
		readEmptyValue(tokeniser, TokenType::ellipsis);
		readEmptyValue(tokeniser, TokenType::dot);
		readNewline(tokeniser);
		console.info("Checking tokenisation of '# Line comment'"sv);
		readValue(tokeniser, TokenType::comment, u8" Line comment"_sv);
		readNewline(tokeniser);
		console.info("Checking tokenisation of '// Other line comment'"sv);
		readValue(tokeniser, TokenType::comment, u8" Other line comment"_sv);
		readNewline(tokeniser);
		console.info("Checking tokenisation of '/**/'"sv);
		readEmptyValue(tokeniser, TokenType::comment);
		readNewline(tokeniser);
		console.info("Checking tokenisation of '/* Partial * line comment */'"sv);
		readValue(tokeniser, TokenType::comment, u8" Partial * line comment "_sv);
		readNewline(tokeniser);
		console.info("Checking tokenisation of '[]'"sv);
		readEmptyValue(tokeniser, TokenType::leftSquare);
		readEmptyValue(tokeniser, TokenType::rightSquare);
		readNewline(tokeniser);
		console.info("Checking tokenisation of '{}'"sv);
		readEmptyValue(tokeniser, TokenType::leftBrace);
		readEmptyValue(tokeniser, TokenType::rightBrace);
		readNewline(tokeniser);
		console.info("Checking tokenisation of '()'"sv);
		readEmptyValue(tokeniser, TokenType::leftParen);
		readEmptyValue(tokeniser, TokenType::rightParen);
		readNewline(tokeniser);
		console.info("Checking tokenisation of ','"sv);
		readEmptyValue(tokeniser, TokenType::comma);
		readNewline(tokeniser);
		console.info("Checking tokenisation of ':'"sv);
		readEmptyValue(tokeniser, TokenType::colon);
		readNewline(tokeniser);
		console.info("Checking tokenisation of ';'"sv);
		readEmptyValue(tokeniser, TokenType::semi);
		readNewline(tokeniser);
		console.info("Checking tokenisation of '<'"sv);
		readValue(tokeniser, TokenType::relOp, u8"<"_sv);
		readNewline(tokeniser);
		console.info("Checking tokenisation of '<='"sv);
		readValue(tokeniser, TokenType::relOp, u8"<="_sv);
		readNewline(tokeniser);
		console.info("Checking tokenisation of '>'"sv);
		readValue(tokeniser, TokenType::relOp, u8">"_sv);
		readNewline(tokeniser);
		console.info("Checking tokenisation of '>='"sv);
		readValue(tokeniser, TokenType::relOp, u8">="_sv);
		readNewline(tokeniser);
		console.info("Checking tokenisation of '!='"sv);
		readValue(tokeniser, TokenType::equOp, u8"!="_sv);
		readNewline(tokeniser);
		console.info("Checking tokenisation of '=='"sv);
		readValue(tokeniser, TokenType::equOp, u8"=="_sv);
		readNewline(tokeniser);
		console.info("Checking tokenisation of '+'"sv);
		readValue(tokeniser, TokenType::addOp, u8"+"_sv);
		readNewline(tokeniser);
		console.info("Checking tokenisation of '-'"sv);
		readValue(tokeniser, TokenType::addOp, u8"-"_sv);
		readNewline(tokeniser);
		console.info("Checking tokenisation of '*'"sv);
		readValue(tokeniser, TokenType::mulOp, u8"*"_sv);
		readNewline(tokeniser);
		console.info("Checking tokenisation of '/'"sv);
		readValue(tokeniser, TokenType::mulOp, u8"/"_sv);
		readNewline(tokeniser);
		console.info("Checking tokenisation of '%'"sv);
		readValue(tokeniser, TokenType::mulOp, u8"%"_sv);
		readNewline(tokeniser);
		console.info("Checking tokenisation of '~'"sv);
		readValue(tokeniser, TokenType::invert, u8"~"_sv);
		readNewline(tokeniser);
		console.info("Checking tokenisation of '!'"sv);
		readValue(tokeniser, TokenType::invert, u8"!"_sv);
		readNewline(tokeniser);
		console.info("Checking tokenisation of '&'"sv);
		readValue(tokeniser, TokenType::bitOp, u8"&"_sv);
		readNewline(tokeniser);
		console.info("Checking tokenisation of '|'"sv);
		readValue(tokeniser, TokenType::bitOp, u8"|"_sv);
		readNewline(tokeniser);
		console.info("Checking tokenisation of '^'"sv);
		readValue(tokeniser, TokenType::bitOp, u8"^"_sv);
		readNewline(tokeniser);
		console.info("Checking tokenisation of '&&'"sv);
		readValue(tokeniser, TokenType::logicOp, u8"&"_sv);
		readNewline(tokeniser);
		console.info("Checking tokenisation of '||'"sv);
		readValue(tokeniser, TokenType::logicOp, u8"|"_sv);
		readNewline(tokeniser);
		console.info("Checking tokenisation of '>>'"sv);
		readValue(tokeniser, TokenType::shiftOp, u8">>"_sv);
		readNewline(tokeniser);
		console.info("Checking tokenisation of '<<'"sv);
		readValue(tokeniser, TokenType::shiftOp, u8"<<"_sv);
		readNewline(tokeniser);
		console.info("Checking tokenisation of '++'"sv);
		readValue(tokeniser, TokenType::incOp, u8"+"_sv);
		readNewline(tokeniser);
		console.info("Checking tokenisation of '--'"sv);
		readValue(tokeniser, TokenType::incOp, u8"-"_sv);
		readNewline(tokeniser);
		console.info("Checking tokenisation of '->'"sv);
		readEmptyValue(tokeniser, TokenType::arrow);
		readNewline(tokeniser);
		// Finally, consume one last token and make sure it's the EOF token
		readEOF(tokeniser);
	}

public:
	void registerTests() final
	{
		console = {stdout, stderr};
		CRUNCHpp_TEST(testBadFileConstruction)
		CRUNCHpp_TEST(testIntegralLiterals)
		CRUNCHpp_TEST(testStringLiterals)
		CRUNCHpp_TEST(testAssignments)
		CRUNCHpp_TEST(testKeywords)
		CRUNCHpp_TEST(testPunctuation)
	}
};

CRUNCHpp_TESTS(testTokeniser)
