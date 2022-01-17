#include "Execute.h"
#include "JtsFunc.h"
#include "Object.h"
#include "JtsType.h"
#include "cppFunc.h"
#include "CppClass.h"
#include "Log.h"
#include "VM.h"

#include "util/ObjectOp.h"

namespace jts {

	Obj* evalObj(VM* vm, Obj* obj) {

		if (!obj) {
			return NIL;
		}

		switch (obj->type) {
			case Type::LIST: {
				
				if (!obj->_args) {
					break;
				}

				switch (obj->_args->val->type) {
					case Type::NAT_FN:
					case Type::JTS_FN:
					case Type::CPP_FN: 

						return execObj(vm, obj->_args);
					
					case Type::QUOTE: 

						if (vm->eval) {
							switch (obj->_args->val->_quote->type) {
								case Type::NAT_FN:
								case Type::JTS_FN:
								case Type::CPP_FN:

									return execObj(vm, obj->_args);
							}
						}

						break;

					default: 

						auto elm = obj->_args;

						while (elm) {
							elm->val = evalObj(vm, elm->val);
							elm = elm->nxt;
						}

						break;
				}

				break;
			}

			case Type::QUOTE:
				if (vm->eval) {
					obj = evalObj(env::setEval(vm, false), obj->_quote);
					env::setEval(vm, true);
				}

				break;
		}

		return obj;
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

		env::pushUsed(vm, ret);

		return ret;
	}
}
