#include "Execute.h"
#include "JtsFunc.h"
#include "Object.h"

namespace jts
{
	Obj* EvalObj(ObjNode* obj)
	{
		if (!obj)
		{
			return NIL;
		}
		if (obj->invocation && obj->value->fnType != FnType::NIL)
		{
			return ExecObj(obj);
		}

		return obj->value;
	}

	Obj* ExecObj(ObjNode* obj)
	{
		switch (obj->value->fnType)
		{
			case FnType::NATIVE:

				return obj->value->_native(obj);

			case FnType::JTS:

				return ExecJtsFunc(obj);

			default: // case C_BRIDGE

				return nullptr;
		}
	}
}
