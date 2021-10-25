#ifndef JTS_FUNC_H
#define JTS_FUNC_H

#include "Types.h"

namespace jts
{
	struct JtsFn
	{
		ObjNode* codeBlock = nullptr;
		
		ObjNode* params    = nullptr;

		Obj* call (ObjNode* args, bool eval);
	};	
}

#endif