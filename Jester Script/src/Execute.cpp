#include "Execute.h"
#include "JtsFunc.h"
#include "Object.h"

namespace jts
{
	Obj* EvalObj(ObjNode* obj)
	{
		if (!obj) return NIL;

		switch (obj->value->spec)
		{
			case Spec::CALL_BEG:

				return ExecObj(obj);

			default:

				return obj->value;
		}
	}

	Obj* ExecObj(ObjNode* obj)
	{
		if (obj->value->type == Type::LIST)
		{
			auto* ret = new ObjNode(); //memory leak
			return obj->value->_list->value->_native(ret, obj->value->_list->next);
		}

		switch (obj->args->value->type)
		{
			case Type::NATIVE:

				return obj->args->value->_native(obj, obj->args->next);

			case Type::JTS:

				return ExecJtsFunc(obj);

			default: // case C_BRIDGE

				return nullptr;
		}
	}
}
