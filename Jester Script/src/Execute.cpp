#include "Execute.h"
#include "JtsFunc.h"
#include "JtsMacro.h"
#include "Object.h"
#include "Operations.h"
#include "cppFunc.h"

namespace jts
{
	Obj* EvalObj(ObjNode* obj, bool eval)
	{
		if (!obj) return NIL;

		switch (obj->value->type)
		{
			case Type::LIST:

				if (!obj->value->_args) return NIL;

				switch (obj->value->_args->value->type)
				{
					case Type::MACRO:
					case Type::NATIVE:
					case Type::JTS_FN:
					case Type::CPP_FN:
						
						return ExecObj(obj->value->ret, obj->value->_args, eval);

					case Type::QUOTE:

						if (eval)
						{
							switch (obj->value->_args->value->_quote->type)
							{
								case Type::NATIVE:
								case Type::JTS_FN:
								case Type::CPP_FN:

									return ExecObj(obj->value->ret, obj->value->_args, eval);
							}
						}

						return obj->value;

					default:

						// iterate over list elements and evaluate

						auto* elem = obj->value->_args;

						while (elem->next)
						{
							// memory leak
							elem->value = EvalObj(elem, eval);
							elem = elem->next;
						}

						return obj->value;
				}

			case Type::QUOTE:

				if (eval) return obj->value->_quote;
				return obj->value;

			default:

				return obj->value;
		}
	}

	Obj* ExecObj(Obj* ret, ObjNode* args, bool eval) 
	{
		switch (args->value->type)
		{
			case Type::NATIVE:

				return args->value->_native(ret, args->next, eval);

			case Type::JTS_FN:

				return args->value->_jtsFunc->Call(args, eval);

			case Type::MACRO:

				return args->value->_jtsMacro->Call(args, eval);

			case Type::CPP_FN:

				return args->value->_cppFunc->Call(ret, args);

			case Type::QUOTE:
				
				if (eval)
				{
					switch (args->value->_quote->type)
					{
						case Type::NATIVE:
						case Type::JTS_FN:
						case Type::CPP_FN:

							// add other cases later
							return args->value->_quote->_native(ret, args->next, eval);
					}
				}

				break;
		}

		return nullptr;
	}
}
