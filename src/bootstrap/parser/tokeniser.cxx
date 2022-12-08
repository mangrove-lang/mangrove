// SPDX-License-Identifier: BSD-3-Clause
#include "tokeniser.hxx"

using namespace mangrove::parser;
using namespace mangrove::parser::types;

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

void Tokeniser::readToken() noexcept
{
}
