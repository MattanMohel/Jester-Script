#ifndef LEXER_H
#define LEXER_H

#include "Types.h"

namespace jts {
	static const char splicers [] = { ' ', '\n', '\t', EOF };
	static const char prefixes [] = { '(', ')', ';' };
	static const char operators[] = { '\'', '~', };

	bool isPrefix  (char value);
	bool isSplicer (char value);
	bool isOperator(char value);
	bool isNonToken(char value);

	void tokenizeFile(VM* vm, str src);

	void addToken(VM* vm, str& symbol, size_t line);

	void matchTokenType(VM* vm);

	static str::iterator scopeTransform(str& src, size_t depth, str::iterator it, std::function<void(int depth, str::iterator it)> trans) {
		int curDepth = depth + 1;

		while (curDepth > depth) {
			++it;
			curDepth += (*it == '(' || *it == '[') - (*it == ')' || *it == ']');

			trans(depth, it);
		}

		return it;
	}

	str::iterator addOperator(VM* vm, str& src, const str& rplc, str::iterator it);
}

#endif