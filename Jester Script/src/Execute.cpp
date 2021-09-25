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
		auto* arg = args;

		while (arg)
		{
			RetOf(arg) = EvalObj(arg, eval);
			arg = arg->next;
		}

		switch (RetOf(args)->type)
		{
			case Type::NATIVE:

				return RetOf(args)->_native(ret, args->next);

			case Type::JTS:

				return ExecJtsFunc(ret);

			default: // case C_BRIDGE

				return nullptr;
		}
	}
}
