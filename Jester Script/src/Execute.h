#ifndef EXECUTE_H
#define EXECUTE_H

#define JTS_DEBUG 1

#include "Types.h"

namespace jts
{
	Obj* EvalObj(ObjNode* obj);
	Obj* ExecObj(ObjNode* obj);
}

#endif