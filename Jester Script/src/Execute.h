#ifndef EXECUTE_H
#define EXECUTE_H

#define JTS_DEBUG 1

#include "Types.h"

namespace jts
{
	Obj* EvalObj(ObjNode* obj, bool eval = false);
	Obj* ExecObj(Obj* ret, ObjNode* args, bool eval);
}

#endif