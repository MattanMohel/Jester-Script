#ifndef EXECUTE_H
#define EXECUTE_H

#include "Types.h"

namespace jts
{
	Obj* EvalObj(ObjNode* obj);
	Obj* ExecObj(ObjNode* obj);
}

#endif