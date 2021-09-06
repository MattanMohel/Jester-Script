#include "Execute.h"
#include "Object.h"

namespace jts
{
	Obj* EvalObj(Obj* obj)
	{
		if (obj->spec == Spec::FUNC)
		{
			return ExecObj(obj);
		}

		return obj;
	}

	Obj* ExecObj(Obj* obj)
	{
		switch (obj->fnType)
		{
			case FnType::NAT_FUNC:

				return obj->_native(obj);

			case FnType::CPP_FUNC:

				return nullptr;

			default: // case JTS_FUNC

				return nullptr;
		}
	}
}
