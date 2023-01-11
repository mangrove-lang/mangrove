// SPDX-License-Identifier: BSD-3-Clause
#include "parser.hxx"

using namespace mangrove::parser;

Parser::Parser(const path &fileName) : lexer{fd_t{fileName.c_str(), O_RDONLY | O_NOCTTY}}
{
	if (!addBuiltinTypesTo(*_symbolTable))
		throw std::exception{};
}
