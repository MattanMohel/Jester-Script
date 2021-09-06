#ifndef TOKEN_H
#define TOKEN_H

#include "Types.h"

namespace jts
{
	struct Tok
	{
		Tok* rest;

		str value;
		size_t line;

		Type type;
		Spec spec;
		FnType fnType;
	};

	Tok* IncrTok(Tok*& tok, size_t index = 1);

	Tok* SeekTok(Tok* tok, size_t index = 1);
}

#endif