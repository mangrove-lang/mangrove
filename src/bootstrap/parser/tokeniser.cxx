// SPDX-License-Identifier: BSD-3-Clause
#include <substrate/conversions>
#include <substrate/console>
#include "tokeniser.hxx"

using namespace mangrove::parser;
using namespace mangrove::parser::types;
using namespace mangrove::parser::recognisers;
using substrate::toInt_t;
using substrate::console;
using namespace std::literals::string_view_literals;

Tokeniser::Tokeniser(fd_t &&file) noexcept : _file{std::move(file)}
{
	_token.endsAt(position);
	nextChar();
}

Token &Tokeniser::next() noexcept
{
	if (_file.isEOF())
	{
		_token.set(TokenType::eof);
		return _token;
	}
	_token.reset();
	readToken();
	return _token;
}

Char Tokeniser::nextChar() noexcept
{
	// Copy the current character
	auto value{currentChar};
	// Handle end-of-file
	if (_file.isEOF())
	{
		currentChar = {};
		return value;
	}

	// Read the next character
	currentChar = {_file};
	// Special-case newline characters
	if (isNewLine(currentChar))
	{
		// If the CR or LF character is on its own of the first part of the windows sequence, update position
		// If this is the LF of a CR-LF sequence, ignore it so windows line endings don't screw up compiler messages.
		if ((value != '\r'_u8c && currentChar == '\n'_u8c) || currentChar == '\r'_u8c)
		{
			++position.line;
			position.character = 0;
		}
	}
	else
		++position.character;
	// return the old character
	return value;
}

void Tokeniser::finaliseToken(const std::optional<TokenType> type, String &&value) noexcept
{
	if (type)
		_token.set(*type, std::move(value));
	_token.endsAt(position);
}

void Tokeniser::readToken() noexcept
{
	switch (currentChar.toCodePoint())
	{
		case ' ':
		case '\t':
			_token.set(TokenType::whitespace);
			break;
		case '#':
			readLineComment();
			return;
		case '\r':
		case '\n':
			_token.set(TokenType::newline);
			break;
		case '.':
			readEllipsisToken();
			break;
		case ';':
			_token.set(TokenType::semi);
			break;
		case '{':
			_token.set(TokenType::leftBrace);
			break;
		case '}':
			_token.set(TokenType::rightBrace);
			break;
		case '(':
			_token.set(TokenType::leftParen);
			break;
		case ')':
			_token.set(TokenType::rightParen);
			break;
		case '[':
			_token.set(TokenType::leftSquare);
			break;
		case ']':
			_token.set(TokenType::rightSquare);
			break;
		case ',':
			_token.set(TokenType::comma);
			break;
		case ':':
			_token.set(TokenType::colon);
			break;
		case '"':
			readStringToken();
			break;
		case '\'':
			readCharToken();
			break;
		case '~':
			_token.set(TokenType::invert);
			break;
		case '/':
			readDivToken();
			return;
		case '*':
		case '%':
			readMulToken();
			return;
		case '+':
		case '-':
			readAddToken();
			return;
		case '&':
		case '|':
			readBooleanToken();
			return;
		case '^':
			readBitwiseToken();
			return;
		case '<':
		case '>':
			readRelationToken();
			return;
		case '!':
		case '=':
			readEqualityToken();
			return;
		default:
			readExtendedToken();
			return;
	}
	finaliseToken();
	nextChar();
}

void Tokeniser::readExtendedToken() noexcept
{
	_token.set(TokenType::ident);
	if (isAlpha(currentChar) || isUnderscore(currentChar))
	{
		auto token{readAlphaNumToken()};
		if (token.isEmpty() || _file.isEOF())
			return;
		if (isTrue(token) || isFalse(token))
			_token.set(TokenType::boolLit);
		else if (isNull(token))
			_token.set(TokenType::nullptrLit);
		else if (token == u8"and"_sv)
			_token.set(TokenType::logicOp, '&'_u8c);
		else if (token == u8"or"_sv)
			_token.set(TokenType::logicOp, '|'_u8c);
		else if (token == u8"not"_sv)
			_token.set(TokenType::logicOp, '!'_u8c);
		else if (isLocationSpec(token))
			_token.set(TokenType::locationSpec);
		else if (isStorageSpec(token))
			_token.set(TokenType::storageSpec);
		else if (isNew(token))
			_token.set(TokenType::newStmt);
		else if (isDelete(token))
			_token.set(TokenType::deleteStmt);
		else if (isFrom(token))
			_token.set(TokenType::fromStmt);
		else if (isImport(token))
			_token.set(TokenType::importStmt);
		else if (isAs(token))
			_token.set(TokenType::asStmt);
		else if (isReturn(token))
			_token.set(TokenType::returnStmt);
		else if (isIfStmt(token))
			_token.set(TokenType::ifStmt);
		else if (isElifStmt(token))
			_token.set(TokenType::elifStmt);
		else if (isElseStmt(token))
			_token.set(TokenType::elseStmt);
		else if (isForStmt(token))
			_token.set(TokenType::forStmt);
		else if (isWhileStmt(token))
			_token.set(TokenType::whileStmt);
		else if (isDoStmt(token))
			_token.set(TokenType::doStmt);

		else if (isNone(token))
			_token.set(TokenType::noneType);
		else if (isClass(token))
			_token.set(TokenType::classDef);
		else if (isEnum(token))
			_token.set(TokenType::enumDef);
		else if (isFunctionDef(token))
			_token.set(TokenType::functionDef);
		else if (isOperatorDef(token))
			_token.set(TokenType::operatorDef);
		else if (isVisibility(token))
			_token.set(TokenType::visibility);
		else if (isUnsafe(token))
			_token.set(TokenType::unsafe);

		// Make sure the token's value is set to the identifier string now we've classified the type
		if (_token.value().isEmpty())
			_token.value(std::move(token));
	}
	else if (isDigit(currentChar))
		readIntToken();
	else
	{
		_token.set(TokenType::invalid);
		nextChar();
	}
}

void Tokeniser::readPartComment() noexcept
{
	_token.set(TokenType::comment);
	auto foundEnd{false};
	String comment{};
	while (!foundEnd && !_file.isEOF())
	{
		if (currentChar == '*'_u8c)
		{
			const auto value{nextChar()};
			if (currentChar == '/'_u8c)
			{
				nextChar();
				foundEnd = true;
			}
			else
				comment += value;
		}
		else
			comment += nextChar();
	}
	finaliseToken(TokenType::comment, std::move(comment));
}

void Tokeniser::readLineComment() noexcept
{
	_token.set(TokenType::comment);
	String comment{};
	while (!_file.isEOF() && !isNewLine(currentChar))
		comment += nextChar();
	finaliseToken(TokenType::comment, std::move(comment));
}

void Tokeniser::readEllipsisToken() noexcept
{
	_token.set(TokenType::dot);
	const auto currentPosition{position};
	const auto offset{_file.tell()};
	nextChar();
	if (nextChar() == '.'_u8c && currentChar == '.'_u8c)
		_token.set(TokenType::ellipsis);
	else
	{
		// Seek back to where we were, taking into account conditions like EOF occuring
		if (_file.seek(offset, SEEK_SET) != offset)
			// We should handle this better, but thought needs to be put into /how/.
			console.error("File seek failed, tokenisation will now be unreliable"sv);
		position = currentPosition;
	}
}

void Tokeniser::readBinToken() noexcept
{
	String literal{};
	_token.set(TokenType::binLit);
	nextChar();
	while (isBin(currentChar))
		literal += nextChar();
	if (literal.isEmpty())
		_token.set(TokenType::invalid);
	else
		_token.value(std::move(literal));
}

void Tokeniser::readOctToken() noexcept
{
	String literal{};
	_token.set(TokenType::octLit);
	nextChar();
	while (isOct(currentChar))
		literal += nextChar();
	if (literal.isEmpty())
		_token.set(TokenType::invalid);
	else
		_token.value(std::move(literal));
}

void Tokeniser::readHexToken() noexcept
{
	String literal{};
	_token.set(TokenType::hexLit);
	nextChar();
	while (isHex(currentChar))
		literal += nextChar();
	if (literal.isEmpty())
		_token.set(TokenType::invalid);
	else
		_token.value(std::move(literal));
}

void Tokeniser::readIntToken() noexcept
{
	String literal{};
	Char firstDigit{currentChar};
	_token.set(TokenType::intLit);
	if (firstDigit == '0'_u8c)
	{
		nextChar();
		if (isBeginBin(currentChar))
			return readBinToken();
		if (isBeginOct(currentChar))
			return readOctToken();
		if (isBeginHex(currentChar))
			return readHexToken();
		literal += firstDigit;
	}
	while (isDigit(currentChar))
		literal += nextChar();
	_token.set(TokenType::intLit, std::move(literal));
}

Char Tokeniser::readUnicode(const Char &normalQuote, const Char &escapedQuote) noexcept
{
	Char result{};
	if (isNormalAlpha(currentChar) || currentChar == normalQuote)
		result = currentChar;
	else if (currentChar == '\\')
	{
		nextChar();
		switch (currentChar.toCodePoint())
		{
			case '\\':
				result = currentChar;
				break;
			case 'b':
				result.fromCodePoint(8U);
				break;
			case 'r':
				result.fromCodePoint(13U);
				break;
			case 'n':
				result.fromCodePoint(10U);
				break;
			case 't':
				result.fromCodePoint(9U);
				break;
			case 'v':
				result.fromCodePoint(11U);
				break;
			case 'f':
				result.fromCodePoint(12U);
				break;
			case 'a':
				result.fromCodePoint(7U);
				break;
			case 'u':
			case 'U': {
				const auto type{_token.type()};
				readHexToken();
				// We get to abuse the StringView abstraction here as we're guaranteed ASCII-only
				// values being present in _token.value() by readHexToken(), so this is safe.
				result = {toInt_t<uint32_t>{_token.value().data(), _token.value().byteLength()}.fromHex()};
				_token.set(type);
				return result;
			}
		}
		if (currentChar == escapedQuote)
			result = escapedQuote;
	}
	nextChar();
	return result;
}

void Tokeniser::readStringToken() noexcept
{
	_token.set(TokenType::stringLit);
	nextChar();
	String literal{};
	while (!isDoubleQuote(currentChar))
	{
		const auto value{readUnicode('\''_u8c, '"'_u8c)};
		if (!value.valid())
		{
			_token.set(TokenType::invalid);
			return;
		}
		literal += value;
	}
	_token.value(std::move(literal));
}

void Tokeniser::readCharToken() noexcept
{
	_token.set(TokenType::charLit);
	nextChar();
	if (isSingleQuote(currentChar))
	{
		_token.set(TokenType::invalid);
		return;
	}
	const auto literal{readUnicode('"'_u8c, '\''_u8c)};
	if (!literal.valid() || !isSingleQuote(currentChar))
	{
		_token.set(TokenType::invalid);
		return;
	}
	_token.value(literal);
}

void Tokeniser::readDivToken() noexcept
{
	finaliseToken(TokenType::mulOp, currentChar);
	String token{nextChar()};
	if (isEquals(currentChar))
	{
		token += currentChar;
		finaliseToken(TokenType::assignOp, std::move(token));
		nextChar();
	}
	else if (currentChar == '*'_u8c)
	{
		nextChar();
		readPartComment();
	}
	else if (currentChar == '/'_u8c)
	{
		nextChar();
		readLineComment();
	}
}

void Tokeniser::readMulToken() noexcept
{
	finaliseToken(TokenType::mulOp, currentChar);
	String token{nextChar()};
	if (isEquals(currentChar))
	{
		token += currentChar;
		finaliseToken(TokenType::assignOp, std::move(token));
		nextChar();
	}
}

void Tokeniser::readAddToken() noexcept
{
	finaliseToken(TokenType::addOp, currentChar);
	const auto token{nextChar()};
	if (isEquals(currentChar))
		finaliseToken(TokenType::assignOp, {token, currentChar});
	else if (token == '-'_u8c && currentChar == '>'_u8c)
		finaliseToken(TokenType::arrow);
	else if (currentChar == token)
		finaliseToken(TokenType::incOp, token);
	else
		return;
	nextChar();
}

void Tokeniser::readBooleanToken() noexcept
{
	finaliseToken(TokenType::bitOp, currentChar);
	const auto token{nextChar()};
	if (isEquals(currentChar))
		finaliseToken(TokenType::assignOp, {token, currentChar});
	else if (currentChar == token)
		finaliseToken(TokenType::logicOp, token);
	else
		return;
	nextChar();
}

void Tokeniser::readBitwiseToken() noexcept
{
	finaliseToken(TokenType::bitOp, currentChar);
	const auto token{nextChar()};
	if (isEquals(currentChar))
	{
		finaliseToken(TokenType::assignOp, {token, currentChar});
		nextChar();
	}
}

void Tokeniser::readRelationToken() noexcept
{
	finaliseToken(TokenType::relOp, currentChar);
	const auto token{nextChar()};
	if (isEquals(currentChar))
		finaliseToken(TokenType::assignOp, {token, currentChar});
	else if (currentChar == token)
	{
		finaliseToken(TokenType::shiftOp, {token, currentChar});
		nextChar();
		if (isEquals(currentChar))
			finaliseToken(TokenType::assignOp, {token, token, currentChar});
		else
			return;
	}
	else
		return;
	nextChar();
}

void Tokeniser::readEqualityToken() noexcept
{
	finaliseToken();
	const auto token{nextChar()};
	if (isEquals(currentChar))
	{
		finaliseToken(TokenType::equOp, {token, currentChar});
		nextChar();
	}
	else
	{
		if (isEquals(token))
			_token.set(TokenType::assignOp, token);
		else
			_token.set(TokenType::invert, token);
	}
}

String Tokeniser::readAlphaNumToken() noexcept
{
	String token{};
	while (isAlphaNum(currentChar) || isUnderscore(currentChar))
	{
		finaliseToken();
		token += nextChar();
	}
	return token;
}
