#ifndef LEXER_H
#define LEXER_H

#include "Types.h"

namespace jts {
	static const char splicers[] = { ' ', '\n', '\t', EOF };
	static const char prefixes[] = { '(', ')', '[', ']', ';' };

	// {char (key), tuple<str (function), size_t (elements)>}
	static std::unordered_map<char, std::tuple<str, size_t>> operators =
	{
		{'\'', {"quote", 1}},
		{'~',  {"eval" , 1}},
		{':',  {"set"  , 2}},
	};

	bool isPrefix(char value);
	bool isSplicer(char value);
	bool isOperator(char value);

	// Takes a string source and tokenizes to VM

	void tokenizeFile(VM* vm, str src);

	// Converts parameters to a Token and appends to VM

	void addToken(VM* vm, str& symbol, size_t line);

	// Matches most recent Token with corresponding object in VM

	void matchTokenType(VM* vm);

	/*
	  Lexically replaces an operator with its function

	  Converts:
	   - '(+ 1 2) into (quote (+ 1 2))
	   - :a :b 10 into (set a (set b 10))
	*/
	str::iterator addOperator(VM* vm, str& src, size_t startDepth, str::iterator cur, bool head = true);
}

#endif