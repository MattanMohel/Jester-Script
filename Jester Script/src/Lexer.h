#ifndef LEXER_H
#define LEXER_H

#include "Types.h"

namespace jts
{
	static const char prefixes[] = { '(', ')', '\'', '#', ',' };

	bool IsPrefix(char value);

	void TokenizeFile(VM* vm, str& src);
	void MatchTokenType(VM* vm);

	str ExtractWord(str src);
}

#endif