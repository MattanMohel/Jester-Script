#ifndef TOKEN_H
#define TOKEN_H

#include "Types.h"

namespace jts
{
	// Used for lexical analysis
	// --Precursor to Obj in parsing hierarchy
	struct Tok
	{
		Tok* next = nullptr;

		str symbol;
		size_t line;

		Type type;
		Spec spec;
	};
}

#endif