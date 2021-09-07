#include "Execute.h"
#include "JtsFunc.h"
#include "Object.h"

namespace jts
{
	Obj* EvalObj(ObjNode* obj)
	{
		if (obj->value->spec == Spec::FUNC)
		{
			return ExecObj(obj);
		}

		return obj->value;
	}

	Obj* ExecObj(ObjNode* obj)
	{
		switch (obj->value->fnType)
		{
			case FnType::NAT_FUNC:

				return obj->value->_native(obj);

			case FnType::JTS_FUNC:

				return ExecJtsFunc(obj);

			default: // case CPP_FUNC

				return nullptr;
		}
	}
}
