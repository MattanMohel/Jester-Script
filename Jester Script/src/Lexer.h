#ifndef LEXER_H
#define LEXER_H

#include "Types.h"

namespace jts
{
	static const char prefixes[] = { '(', ')', '\'', ';', ',', '~', '[', ']' };

	bool isPrefix(char value);

	// Takes a string source and tokenizes to VM
	void tokenizeFile(VM* vm, str src);

	// Converts parameters to a Token and appends to VM
	void addToken(VM* vm, str& symbol, size_t line);

	// Matches most recent Token with corresponding object in VM
	void matchTokenType(VM* vm);

	/*
	* Takes a special opertor and lexically replaces it
	* in the src to the counterpart function
	*/ 
	str::iterator addSpecialOp(VM* vm, str op, str& src, size_t& depth, int line, str::iterator cur);
}

#endif