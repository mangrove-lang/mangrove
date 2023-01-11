// SPDX-License-Identifier: BSD-3-Clause
#include <substrate/utility>
#include "symbolTable.hxx"

using namespace mangrove::core::utf8::literals;
using namespace mangrove::ast::symbolTable;

static const auto builtinTypes
{
	substrate::make_array<Symbol>
	({
		{{u8"type"_sv}, {{SymbolTypes::type}}},
		{{u8"none"_sv}, {{SymbolTypes::type, SymbolTypes::none}}},
		{{u8"auto"_sv}, {{SymbolTypes::type, SymbolTypes::autoVal}}},
		{{u8"Bool"_sv}, {{SymbolTypes::type, SymbolTypes::boolVal}}},
		{{u8"Int8"_sv}, {{SymbolTypes::type, SymbolTypes::signedVal, SymbolTypes::int8Bit}}},
		{{u8"Int16"_sv}, {{SymbolTypes::type, SymbolTypes::signedVal, SymbolTypes::int16Bit}}},
		{{u8"Int32"_sv}, {{SymbolTypes::type, SymbolTypes::signedVal, SymbolTypes::int32Bit}}},
		{{u8"Int64"_sv}, {{SymbolTypes::type, SymbolTypes::signedVal, SymbolTypes::int64Bit}}},
		{{u8"UInt8"_sv}, {{SymbolTypes::type, SymbolTypes::unsignedVal, SymbolTypes::int8Bit}}},
		{{u8"UInt16"_sv}, {{SymbolTypes::type, SymbolTypes::unsignedVal, SymbolTypes::int16Bit}}},
		{{u8"UInt32"_sv}, {{SymbolTypes::type, SymbolTypes::unsignedVal, SymbolTypes::int32Bit}}},
		{{u8"UInt64"_sv}, {{SymbolTypes::type, SymbolTypes::unsignedVal, SymbolTypes::int64Bit}}},
		{{u8"Char"_sv}, {{SymbolTypes::type, SymbolTypes::character}}},
		{{u8"String"_sv}, {{SymbolTypes::type, SymbolTypes::character, SymbolTypes::list}}},
		{{u8"List"_sv}, {{SymbolTypes::type, SymbolTypes::list}}},
		{{u8"Array"_sv}, {{SymbolTypes::type, SymbolTypes::array}}},
		{{u8"Dict"_sv}, {{SymbolTypes::type, SymbolTypes::structVal, SymbolTypes::list}}},
		{{u8"Set"_sv}, {{SymbolTypes::type, SymbolTypes::structVal, SymbolTypes::array}}},
	})
};

bool addBuiltinTypesTo(SymbolTable &symbolTable) noexcept
{
	auto result{true};
	for (const auto &type : builtinTypes)
		result &= symbolTable.insert(type);
	return result;
}
