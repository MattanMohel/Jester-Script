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

	Obj* evalObj(VM* vm, Obj* obj) {	
		if (!obj) {
			return NIL;
		}

		switch (obj->type) {
		case Type::LIST:

			if (!obj->_args) {
				break;
			}

			switch (obj->_args->val->type) {
			case Type::NAT_FN:
			case Type::JTS_FN:
			case Type::CPP_FN:

				// if first argument is a callable
				return execObj(vm, obj->_args);

			case Type::QUOTE:

				// evaluate quote only if eval is true

				if (vm->eval) {
					switch (obj->_args->val->_quote->type) {
					case Type::NAT_FN:
					case Type::JTS_FN:
					case Type::CPP_FN:

						// if first argument's quote value is a callable
						return execObj(vm, obj->_args);
					}
				}

				return obj;

			default:

				// iterate over list and evaluate elements

				auto elm = obj->_args;

				while (elm) {
					elm->val = evalObj(vm, elm->val);
					elm = elm->nxt;
				}

				return obj;
			}

		case Type::QUOTE:

			// if a quote to be eval'd, return the quote value, otherwise return the quote

			if (vm->eval) {
				vm->eval = false;
				obj = evalObj(vm, obj->_quote);
				vm->eval = true;
			}

			return obj;

		default:

			// if not a list, callable or quote, return self

			return obj;
		}
	}

	Obj* execObj(VM* vm, Node* args) {

		Obj* ret = nullptr;

		switch (args->val->type) {
		case Type::NAT_FN:

			ret = args->val->_native(vm, args->nxt);
			break;

		case Type::CPP_FN:

			ret = args->val->_cppFn->call(vm, args);
			break;

		case Type::JTS_FN:

 			ret = args->val->_jtsFn->call(vm, args->nxt);
			break;

		case Type::QUOTE:
			switch (args->val->_quote->type) {
			case Type::NAT_FN:

				ret = args->val->_quote->_native(vm, args->nxt);
				break;

			case Type::CPP_FN:

				ret = args->val->_quote->_cppFn->call(vm, args);
				break;

			case Type::JTS_FN:

				ret = args->val->_quote->_jtsFn->call(vm, args->nxt);
				break;
			}
		}



		return ret;
	}
}
