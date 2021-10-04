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
	Obj* EvalObj(Obj* obj, bool eval)
	{
		if (!obj) return NIL;

		switch (obj->type)
		{
			case Type::LIST:

				// if list has no arguments
				if (!obj->_args) return NIL;

				switch (obj->_args->value->type)
				{
					case Type::MAC_FN:
					case Type::NAT_FN:
					case Type::JTS_FN:
					case Type::CPP_FN:
						
						// if first argument is a callable
						return ExecObj(obj->_args, eval);

					case Type::QUOTE:

						// evaluate quote only if eval is true

						if (eval)
						{
							switch (obj->_args->value->_quote->type)
							{
								case Type::NAT_FN:
								case Type::JTS_FN:
								case Type::CPP_FN:

						            // if first argument's quote value is a callable
									return ExecObj(obj->_args, eval);
							}
						}

						return obj;

					default:

						// iterate over list elements and evaluate if a non-callable list

						auto* elem = obj->_args;

						while (elem->next)
						{
							elem->value = EvalObj(elem->value, eval);
							elem = elem->next;
						}

						return obj;
				}

			case Type::QUOTE:

				// if a quote to be eval'd, return the quote value, otherwise return the quote

				if (eval) return EvalObj(obj->_quote, false);
				return obj;

			default:

				// if not a list, callable or quote, return self
				return obj;
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

							retVal = args->value->_quote->_native(ret, args->next, eval);
							break;

						case Type::JTS_FN:

							retVal = args->value->_quote->_jtsFunc->Call(args, eval);
							break;				
						
						case Type::MAC_FN:

							retVal = args->value->_quote->_jtsMacro->Call(args, eval);
							break;

						case Type::CPP_FN:

							retVal = args->value->_quote->_cppFunc->Call(ret, args);
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
