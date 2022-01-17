#ifndef TOKEN_H
#define TOKEN_H

#include "Types.h"

namespace jts {

	// Jester Script Token struct

	struct Tok {
		Tok* next = nullptr;

		str symbol = "";
		size_t line = 0;

		Type type = Type::NIL;
		Spec spec = Spec::NIL;
	};
}

#endif