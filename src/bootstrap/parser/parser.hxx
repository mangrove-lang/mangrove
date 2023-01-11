// SPDX-License-Identifier: BSD-3-Clause
#ifndef PARSER_PARSER_HXX
#define PARSER_PARSER_HXX

#include <filesystem>
#include "tokeniser.hxx"
#include "../ast/symbolTable.hxx"

namespace mangrove::parser
{
	inline namespace internal
	{
		using std::filesystem::path;
		using mangrove::ast::symbolTable::SymbolTable;
	}

	struct Parser
	{
	private:
		Tokeniser lexer;
		std::shared_ptr<SymbolTable> _symbolTable;

	public:
		Parser(path fileName);

		[[nodiscard]] std::weak_ptr<SymbolTable> symbolTable() const noexcept { return _symbolTable; }
	};
} // namespace mangrove::parser

#endif /*PARSER_PARSER_HXX*/
