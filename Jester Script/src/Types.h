#ifndef TYPES_H
#define TYPES_H

#include <string>
#include <unordered_map>
#include <bitset>

#define ENUM_SIZE(e) (size_t)e::END

namespace jts
{
	using str = std::string;

	struct VM;
	struct Tok;
	struct Obj;
	struct Func;
	struct ObjNode;

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
		C_BRIDGE,
		JTS,

		// needs attending to
		STRING,
	};

	enum class Spec
	{
		NIL = 0,

		SYMBOL,
		QUOTE,
		VALUE,
		FLAG,

		CALL_BEG,
		CALL_END,
	};

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
