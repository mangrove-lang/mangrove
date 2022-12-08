// SPDX-License-Identifier: BSD-3-Clause
#include "tokeniser.hxx"

using namespace mangrove::parser;
using namespace mangrove::parser::types;
using mangrove::parser::recognisers::isNewLine;

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

void Tokeniser::readToken() noexcept
{
}
