#ifndef LEXER_H
#define LEXER_H

#include "Types.h"

namespace jts
{
	void TokenizeFile(VM* vm, FILE* file);
	void MatchTokenType(VM* vm);
}

#endif
