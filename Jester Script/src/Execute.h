#ifndef EXECUTE_H
#define EXECUTE_H

#define JTS_DEBUG 1
#define RetOf(objNode) objNode->value->ret->value

#include "Types.h"

namespace jts
{
	Obj* EvalObj(ObjNode* obj, bool eval = false);
	Obj* ExecObj(ObjNode* ret, ObjNode* args, bool eval);
}

#endif