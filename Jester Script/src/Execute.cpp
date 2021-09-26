#include "Execute.h"
#include "JtsFunc.h"
#include "Object.h"
#include "Operations.h"

namespace jts
{
	Obj* EvalObj(ObjNode* obj, bool eval)
	{
		if (!obj) return NIL;

		switch (obj->value->type)
		{
			case Type::LIST:

				switch (obj->value->_args->value->type)
				{
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

				return ExecJtsFunc(ret);

			case Type::CPP_FN:

				return nullptr;

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
