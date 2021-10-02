#ifndef LEXER_H
#define LEXER_H

#include "Types.h"

namespace jts
{
	static const char prefixes[] = { '(', ')', '\'', ';', ',' };

	bool isPrefix(char value);

	// Takes a string source and tokenizes to VM
	void TokenizeFile(VM* vm, str src);

	// Converts parameters to a Token and appends to VM
	void AddToken(VM* vm, str& symbol, size_t line);

	// Matches most recent Token with corresponding object in VM
	void MatchTokenType(VM* vm);
}

#endif