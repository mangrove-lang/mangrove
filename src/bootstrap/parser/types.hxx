// SPDX-License-Identifier: BSD-3-Clause
#ifndef PARSER_TYPES_HXX
#define PARSER_TYPES_HXX

#include <cstdint>

namespace mangrove::parser::types
{
	enum class tokenType_t : uint16_t
	{
		invalid,
		eof,
		whitespace,
		comment,
		newline,
		dot,
		ellipsis,
		semi,
		ident,
		leftParen,
		rightParen,
		leftBrace,
		rightBrace,
		leftSquare,
		rightSquare,
		comma,
		colon,
		binLit,
		octLit,
		hexLit,
		intLit,
		stringLit,
		charLit,
		boolLit,
		float32Lit,
		float64Lit,
		nullptrLit,
		invert,
		incOp,
		mulOp,
		addOp,
		shiftOp,
		bitOp,
		relOp,
		equOp,
		logicOp,

		locationSpec,
		storageSpec,
		type,
		assignOp,

		fromStmt,
		importStmt,
		asStmt,
		newStmt,
		deleteStmt,
		returnStmt,
		ifStmt,
		elifStmt,
		elseStmt,
		forStmt,
		whileStmt,
		doStmt,

		noneType,
		arrow,
		classDef,
		enumDef,
		functionDef,
		operatorDef,
		decorator,
		visibility,
		unsafe
	};
} // namespace mangrove::parser::types

#endif /*PARSER_TYPES_HXX*/
