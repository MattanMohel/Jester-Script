#include "Execute.h"
#include "JtsFunc.h"
#include "Object.h"

namespace jts
{
	Obj* EvalObj(ObjNode* obj, bool eval)
	{
		if (!obj) return NIL;

		if (!eval && obj->value->type == Type::LIST) return obj->value;

		switch (obj->value->spec)
		{
			case Spec::CALL_BEG:

				return ExecObj(obj->value->ret, obj->value->_args, eval);

			default:

				return obj->value;
		}
	}

	Obj* ExecObj(ObjNode* ret, ObjNode* args, bool eval) 
	{
		switch (args->value->type)
		{
			case Type::NATIVE:

				return args->value->_native(ret, args->next);

			case Type::JTS:

				return ExecJtsFunc(ret);

			default: // case C_BRIDGE

				return nullptr;
		}
	}
}
