#ifndef TYPES_H
#define TYPES_H

#include <string>
#include <unordered_map>

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
		STRING,
	};

	enum class Spec
	{
		NIL = 0,

		SYMBOL,

		VALUE,
		CALL,
		FUNC,

		PARENTH_L,
		PARENTH_R,
	};

	enum class FnType
	{
		NIL = 0,

		NATIVE,
		C_BRIDGE,
		JTS,
	};

	enum class Flag
	{
		NIL = 0,

		QUOTE,
		QUASI_QUOTE,
		UNQUOTE,
	};
}

#endif
