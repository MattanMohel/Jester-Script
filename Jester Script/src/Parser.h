#ifndef PARSER_H
#define PARSER_H

#include "Types.h"

namespace jts
{
	void ParseTokens(VM* vm);
	Obj* ParseTokens_Impl(VM* vm, Obj* head, Tok*& it, int& codeDepth, Obj** nextNode);
}

#endif
