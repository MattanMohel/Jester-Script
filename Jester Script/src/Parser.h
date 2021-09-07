#ifndef PARSER_H
#define PARSER_H

#include "Types.h"

namespace jts
{
	void ParseTokens(VM* vm);
	void ParseTokens_Impl(VM* vm, ObjNode* head, Tok*& it, int& codeDepth, ObjNode** nextNode);
}

#endif
