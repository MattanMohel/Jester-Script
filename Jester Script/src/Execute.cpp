#include "Execute.h"
#include "JtsFunc.h"
#include "JtsMacro.h"
#include "Object.h"
#include "Operations.h"
#include "cppFunc.h"
#include "VM.h"

namespace jts
{
	Obj* EvalObj(ObjNode* obj, bool eval)
	{
		if (!obj) return NIL;

		switch (obj->value->type)
		{
			case Type::LIST:

				// if object has no arguments, equivalent to "()", return nil

				if (!obj->value->_args) return NIL;

				switch (obj->value->_args->value->type)
				{
					case Type::MACRO:
					case Type::NATIVE:
					case Type::JTS_FN:
					case Type::CPP_FN:
						
						// if first argument is a callable, return executed object

						return ExecObj(obj->value->_args, eval);

					case Type::QUOTE:

						// evaluate quote only if eval is true

						if (eval)
						{
							switch (obj->value->_args->value->_quote->type)
							{
								case Type::NATIVE:
								case Type::JTS_FN:
								case Type::CPP_FN:

						            // if first argument's quote value is a callable, return executed object
									return ExecObj(obj->value->_args, eval);
							}
						}

						// if a quote but not to be eval'd, return quote value

						return obj->value;

					default:

						// iterate over list elements and evaluate if a non-callable list

						auto* elem = obj->value->_args;

						while (elem->next)
						{
							elem->value = EvalObj(elem, eval);
							elem = elem->next;
						}

						// return the evaluated list

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

		if (args->value->ret) env::glbl_objectPool.release(args->value->ret);

		args->value->ret = env::glbl_objectPool.acquire();

		switch (args->value->type)
		{
			case Type::NATIVE:

				return args->value->_native(args->value->ret, args->next, eval);

			case Type::JTS_FN:

				return args->value->_jtsFunc->Call(args, eval);

			case Type::MACRO:

				return args->value->_jtsMacro->Call(args, eval);

			case Type::CPP_FN:

				return args->value->_cppFunc->Call(args->value->ret, args);

			case Type::QUOTE:
				
				// switch and execute quote value if to be eval'd

				if (eval)
				{
					switch (args->value->_quote->type)
					{
						case Type::NATIVE:

							return args->value->_native(args->value->ret, args->next, eval);

						case Type::JTS_FN:

							return args->value->_jtsFunc->Call(args, eval);

						case Type::CPP_FN:

							return args->value->_cppFunc->Call(args->value->ret, args);
					}
				}

				break;
		}

		// return nullptr if not a callable -- would causes crash

		return nullptr;
	}
}
