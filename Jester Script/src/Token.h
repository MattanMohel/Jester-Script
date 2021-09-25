#ifndef TOKEN_H
#define TOKEN_H

#include "Types.h"

namespace jts
{
	struct Tok
	{
		Tok* next = nullptr;

		str value;
		size_t line;

		Type type;
		Spec spec;
	};
}

#endif