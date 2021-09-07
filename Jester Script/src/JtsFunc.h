#ifndef JTS_FUNC_H
#define JTS_FUNC_H

#include "Types.h"

namespace jts
{
	struct Func
	{
		std::unordered_map<str, Obj*> parameters;

		ObjNode* codeBlock = nullptr;
	};

	Obj* ExecJtsFunc(ObjNode* func);
}

#endif