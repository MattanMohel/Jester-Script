#include "Execute.h"
#include "JtsFunc.h"
#include "Object.h"
#include "Operations.h"
#include "JtsType.h"
#include "cppFunc.h"
#include "CppClass.h"
#include "VM.h"

#include "Log.h"

namespace jts {

	Obj* evalObj(Obj* obj, bool eval, bool top) {
		
		if (!obj) return NIL;

		switch (obj->type) {
		case Type::LIST:

			if (!obj->_args) return obj;

			switch (obj->_args->value->type) {
			case Type::NAT_FN:
			case Type::JTS_FN:
			case Type::CPP_FN:

				// if first argument is a callable
				return execObj(obj->_args, eval);

			case Type::QUOTE:

				// evaluate quote only if eval is true

				if (eval) {
					switch (obj->_args->value->_quote->type) {
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

				while (elem) {
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

	Obj* execObj(ObjNode* args, bool eval, bool top) {
		Obj* retVal = nullptr;

		switch (args->value->type) {
		case Type::NAT_FN:

			retVal = args->value->_native(env::glbl_objPool.acquire(),
										  args->next, eval);
			break;

		case Type::CPP_FN:

			retVal = args->value->_cppFn->call(env::glbl_objPool.acquire(),
											   args);
			break;

		case Type::JTS_FN:

			retVal = args->value->_jtsFn->call(args->next, eval);
			break;

		case Type::QUOTE:
		{
			switch (args->value->_quote->type) {
			case Type::NAT_FN:

				retVal = args->value->_quote->_native(env::glbl_objPool.acquire(),
													  args->next, eval);
				break;

			case Type::CPP_FN:

				retVal = args->value->_quote->_cppFn->call(env::glbl_objPool.acquire(),
														   args);
				break;

			case Type::JTS_FN:

				retVal = args->value->_quote->_jtsFn->call(args->next, eval);
				break;
			}
		}
		}

		if (top) env::glbl_objPool.release_all();

		return retVal;
	}
}
