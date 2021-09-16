#ifndef JTS_FUNC_H
#define JTS_FUNC_H

#include "Types.h"

namespace jts
{
	struct Func
	{
		ObjNode* codeBlock = nullptr;
		
		ObjNode* params    = nullptr;
	};

	Obj* ExecJtsFunc(ObjNode* func);
}

#endif