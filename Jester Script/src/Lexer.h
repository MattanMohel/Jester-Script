#ifndef LEXER_H
#define LEXER_H

#include "Types.h"

namespace jts
{
	static const char prefixes[] = { '(', ')', '\'', '#', ',' };

	bool IsPrefix(char value);

	void TokenizeFile(VM* vm, FILE* file);
	void MatchTokenType(VM* vm);
}

#endif