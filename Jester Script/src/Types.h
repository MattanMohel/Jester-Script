#ifndef TYPES_H
#define TYPES_H

#include <string>
#include <unordered_map>
#include <bitset>

#include "../utils/IterableStack.h"

#define ENUM_SIZE(e) (size_t)e::END

namespace jts
{
	using str = std::string;

	// JTS integral types

	using j_int = int;
	using j_bool = bool;
	using j_char = char;
	using j_float = float;

	struct VM;
	struct Tok;
	struct Obj;
	struct JtsFn;
	struct MacFn;
	struct CppFn;
	struct ObjNode;

	// Symbol type
	enum class Type
	{
		NIL = 0,

		CHAR,
		BOOL,
		INT,
		FLOAT,
		QUOTE,
		LIST,

		NAT_FN,
		CPP_FN,
		JTS_FN,
		MAC_FN,

		STRING,
	};

	// Symbol specialization
	enum class Spec
	{
		NIL = 0,

		SYMBOL, // variable
		VALUE,  // literal value

		LST_BEG, // '('
		LST_END, // ')'

		SCP_BEG, // '['
		SCP_END, // ']'
	};

	// Symbol flags
	enum class SFlag
	{
		NIL = 0,

		STAT,
		CONST,
		MUT,

		END,
	};

	enum class State
	{
		NIL = 0,

		MES, // message
		WRN, // warning
		ERR, // error
	};
}

#endif
