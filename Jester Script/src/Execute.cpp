#include "Execute.h"
#include "JtsFunc.h"
#include "JtsMacro.h"
#include "Object.h"
#include "Operations.h"
#include "JtsType.h"
#include "cppFunc.h"
#include "VM.h"

#include "Log.h"

namespace jts
{
	Obj* evalObj(Obj* obj, bool eval, bool top)
	{
		if (!obj) return NIL;

		switch (obj->type)
		{
			case Type::LIST:

				// if list has no arguments
				if (!obj->_args) return obj;

				switch (obj->_args->value->type)
				{
					case Type::MAC_FN:
					case Type::NAT_FN:
					case Type::JTS_FN:
					case Type::CPP_FN:
						
						// if first argument is a callable
						return execObj(obj->_args, eval);

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
									return execObj(obj->_args, eval);
							}
						}

						return obj;

					default:

						// iterate over list and evaluate elements

						auto* elem = obj->_args;

						while (elem)
						{
							elem->value = evalObj(elem->value, eval);
							elem = elem->next;
						}

						return obj;
				}

			case Type::QUOTE:

				// if a quote to be eval'd, return the quote value, otherwise return the quote

				if (eval) return evalObj(obj->_quote, false);
				return obj;

			default:

				// if not a list, callable or quote, return self

				return obj;
		}
	}

	Obj* execObj(ObjNode* args, bool eval, bool top)
	{
		/*
			Execute object by the callable type

			The head of the passed arguments MUST be of a callkable type
		*/

		Obj* retVal = nullptr;

		Obj* ret = nullptr;

		switch (args->value->type)
		{
			case Type::NAT_FN:

				ret = env::glbl_objPool.acquire();

				retVal = args->value->_native(ret, args->next, eval);
				break;

			case Type::CPP_FN:

				ret = env::glbl_objPool.acquire();

				retVal = args->value->_cppFn->call(ret, args);
				break;

			case Type::JTS_FN:

				retVal = args->value->_jtsFn->call(args->next, eval);
				break;

			case Type::MAC_FN:

				retVal = args->value->_macFn->call(args->next, eval);
				break;


			case Type::QUOTE:
				
				// switch and execute quote value if to be eval'd

				if (eval)
				{
					switch (args->value->_quote->type)
					{
						case Type::NAT_FN:

							ret = env::glbl_objPool.acquire();

							retVal = args->value->_quote->_native(ret, args->next, eval);
							break;

						case Type::CPP_FN:

							ret = env::glbl_objPool.acquire();

							retVal = args->value->_quote->_cppFn->call(ret, args);
							break;

						case Type::JTS_FN:

							retVal = args->value->_quote->_jtsFn->call(args, eval);
							break;				
						
						case Type::MAC_FN:

							retVal = args->value->_quote->_macFn->call(args, eval);
							break;
					}
				}

				break;
		}

		// return nullptr if not a callable -- inflicts crash

		if (top) env::glbl_objPool.release_all();

		return retVal;
	}

	str toString(Obj* obj)
	{
		switch (obj->type)
		{
			case Type::NIL:

				return "nil";

			case Type::STRING:

				return *obj->_string;

			case Type::FLOAT:
			{
				str flt = std::to_string(obj->_float);
				flt.erase(flt.find_last_not_of('0') + 1, str::npos);

				return flt;
			}

			case Type::INT:

				return std::to_string(obj->_int);

			case Type::CHAR:

				return str(1, obj->_char);

			case Type::BOOL:

				return obj->_bool? "true" : "false";

			case Type::QUOTE:

				return obj->_quote->symbol;

			case Type::JTS_TYPE:
			{
				str ret = "";

				for (auto& member : obj->_jtsType->members)
				{
					ret += member.first + ": " + toString(member.second) + " ";
				}

				return ret;
			}

			default:

				return obj->symbol;
		}
	}
}
