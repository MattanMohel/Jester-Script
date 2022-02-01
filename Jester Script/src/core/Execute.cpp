#include "Execute.h"
#include "JtsFunc.h"
#include "Object.h"
#include "JtsType.h"
#include "cppFunc.h"
#include "CppClass.h"
#include "Log.h"
#include "VM.h"

#include "util/ObjectOp.h"
#include "util/ListOp.h"

namespace jts {

	template<> Obj* evalObj<true>(VM* vm, Obj* obj) {
		switch (obj->type) {
		case Type::LIST:
			if (!obj->_args) {
				break;
			}

			if (is<Var::CALLABLE_V>(obj->_args->val) || is<Var::CALLABLE_Q>(obj->_args->val)) {
				return execObj(vm, obj->_args);
			}

			lst::evalSelf(vm, obj->_args);

			break;

		case Type::QUOTE:
			if (vm->eval) {
				obj = evalObj<true>(env::withEval(vm, false), obj->_quote);
				env::withEval(vm, true);
			}

			break;
		}

		return env::newObj(vm, obj);
	}

	template<> Obj* evalObj<false>(VM* vm, Obj* obj) {
		switch (obj->type) {
		case Type::LIST:
			if (!obj->_args) {
				break;
			}

			if (is<Var::CALLABLE_V>(obj->_args->val) || is<Var::CALLABLE_Q>(obj->_args->val)) {
				return env::releaseObj(vm, execObj(vm, obj->_args));
			}

			lst::evalSelf(vm, obj->_args);

			break;

		case Type::QUOTE:
			if (vm->eval) {
				obj = evalObj<true>(env::withEval(vm, false), obj->_quote);
				env::withEval(vm, true);
			}

			break;
		}

		return obj;
	}

	Obj* execObj(VM* vm, Node* args) {
		switch (args->val->type) {

		case Type::CPP_FN:

			return args->val->_cppFn->call(vm, args);

		case Type::NAT_FN:

			return call(vm, args->nxt, args->val->_natFn);

		case Type::JTS_FN:

			return call(vm, args->nxt, args->val->_jtsFn);

		case Type::QUOTE:

			switch (args->val->_quote->type) {

			case Type::CPP_FN:

				return args->val->_quote->_cppFn->call(vm, args);

			case Type::NAT_FN:

				return call(vm, args->nxt, args->val->_natFn);

			case Type::JTS_FN:

				return call(vm, args->nxt, args->val->_jtsFn);
			}
		}

		return nullptr;
	}
}
