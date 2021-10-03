#ifndef TYPES_H
#define TYPES_H

#include <string>
#include <unordered_map>
#include <bitset>

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
	struct Func;
	struct Macro;
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

		NATIVE,
		CPP_FN,
		JTS_FN,
		MACRO,

		STRING,
	};

	// Symbol specialization
	enum class Spec
	{
		NIL = 0,

		SYMBOL,
		VALUE,

		BEG,
		END,
	};

	// Symbol flags
	enum class SFlag
	{
		NIL = 0,

		CONST,
		STATIC,
		REF, 

		END,
	};
}

#endif
