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

		NAT_FUNC,
		CPP_FUNC,
		JTS_FUNC,
	};

	enum class Spec
	{
		NIL = 0,

		SYMBOL,

		LTRL,
		VAR,
		FUNC,

		PARENTH_L,
		PARENTH_R,
		BRACKET_L,
		BRACKET_R,
	};

	enum class FnType
	{
		NIL = 0,

		NAT_FUNC,
		CPP_FUNC,
		JTS_FUNC,
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
