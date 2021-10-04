#include "Execute.h"
#include "JtsFunc.h"
#include "JtsMacro.h"
#include "Object.h"
#include "Operations.h"
#include "cppFunc.h"
#include "VM.h"

#include "Log.h"

namespace jts
{
	Obj* EvalObj(ObjNode* obj, bool eval)
	{
		if (!obj) return NIL;

		switch (obj->value->type)
		{
			case Type::LIST:

				// if list has no arguments
				if (!obj->value->_args) return NIL;

				switch (obj->value->_args->value->type)
				{
					case Type::MAC_FN:
					case Type::NAT_FN:
					case Type::JTS_FN:
					case Type::CPP_FN:
						
						// if first argument is a callable
						return ExecObj(obj->value->_args, eval);

					case Type::QUOTE:

						// evaluate quote only if eval is true

						if (eval)
						{
							switch (obj->value->_args->value->_quote->type)
							{
								case Type::NAT_FN:
								case Type::JTS_FN:
								case Type::CPP_FN:

						            // if first argument's quote value is a callable
									return ExecObj(obj->value->_args, eval);
							}
						}

						return obj->value;

					default:

						// iterate over list elements and evaluate if a non-callable list

						auto* elem = obj->value->_args;

						while (elem->next)
						{
							elem->value = EvalObj(elem, eval);
							elem = elem->next;
						}

						return obj->value;
				}

			case Type::QUOTE:

				// if a quote to be eval'd, return the quote value, otherwise return the quote

				if (eval) return obj->value->_quote;
				return obj->value;

			default:

				// if not a list, callable or quote, return self
				return obj->value;
		}
	}

	Obj* ExecObj(ObjNode* args, bool eval)
	{
		/*
			Execute object by the callable type

			The head of the passed arguments MUST be of a callkable type
		*/

		Obj* retVal = nullptr;

		Obj* ret = env::glbl_objPool.acquire();

		switch (args->value->type)
		{
			case Type::NAT_FN:

				retVal = args->value->_native(ret, args->next, eval);
				break;

			case Type::JTS_FN:

				retVal = args->value->_jtsFunc->Call(args, eval);
				break;

			case Type::MAC_FN:

				retVal = args->value->_jtsMacro->Call(args, eval);
				break;

			case Type::CPP_FN:

				retVal = args->value->_cppFunc->Call(ret, args);
				break;

			case Type::QUOTE:
				
				// switch and execute quote value if to be eval'd

				if (eval)
				{
					switch (args->value->_quote->type)
					{
						case Type::NAT_FN:

							retVal = args->value->_native(ret, args->next, eval);
							break;

						case Type::JTS_FN:

							retVal = args->value->_jtsFunc->Call(args, eval);
							break;

						case Type::CPP_FN:

							retVal = args->value->_cppFunc->Call(ret, args);
							break;
					}
				}

				break;
		}

		// return nullptr if not a callable -- inflicts crash

		env::glbl_objPool.release(ret);

		return retVal;
	}
}
