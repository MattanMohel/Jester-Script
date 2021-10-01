#ifndef JTS_MACRO_H
#define JTS_MACRO_H

#include "Types.h"

namespace jts
{
	struct Macro
	{
		ObjNode* codeBlock = nullptr;

		ObjNode* params = nullptr;

		Obj* Call (ObjNode* args, bool eval);
	};
}

#endif
