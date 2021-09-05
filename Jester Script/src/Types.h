#ifndef TYPES_H
#define TYPES_H

#include <string>

namespace jts
{
	using str = std::string;

	struct VM;

	enum class Type
	{
		VOID,
		CHAR,
		BOOL,
		INT,
		FLOAT,
	};

	enum class Spec
	{
		LTRL,
		VAR,
		FUNC,
	};

	enum class FnType
	{
		NAT_FUNC,
		CPP_FUNC,
		JTS_FUNC,
	};

	enum class Flag
	{
		CONST,
		STATIC,

		QUOTE,
		QUASI_QUOTE,
		UNQUOTE,
	};

	struct Tok
	{
		Tok* head;

		str value;
		size_t line;
	};

}
#endif
