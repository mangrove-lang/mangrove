// SPDX-License-Identifier: BSD-3-Clause
#include "tokeniser.hxx"

using namespace mangrove::parser;
using namespace mangrove::parser::types;
using namespace mangrove::parser::recognisers;

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
		if ((value != '\r' && currentChar == '\n') || currentChar == '\r')
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
	//_token.calcLength();
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

		case '~':
			_token.set(TokenType::invert);
			break;
	}
	finaliseToken();
	nextChar();
}

void Tokeniser::readLineComment() noexcept
{
	_token.set(TokenType::comment);
	String comment{};
	while (!_file.isEOF() && !isNewLine(currentChar))
		comment += nextChar();
	finaliseToken(TokenType::comment, std::move(comment));
}

void Tokeniser::readHexToken() noexcept
{
	String literal{};
	_token.set(TokenType::hexLit);
	nextChar();
	while (isHex(currentChar))
		literal += nextChar();
	_token.set(TokenType::hexLit, std::move(literal));
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
			case 'U':
				readHexToken();
				//return {toInt_t{_token.value.data(), _token.value.byteLength()}.fromHex()};
				return {};
		}
		if (currentChar == escapedQuote)
			result = escapedQuote;
	}
	nextChar();
	return result;
}

