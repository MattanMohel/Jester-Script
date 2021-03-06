#ifndef STANDARD_H
#define STANDARD_H

#include "core/VM.h"
#include "core/Types.h"
#include "core/Object.h"
#include "core/Execute.h"
#include "core/JtsFunc.h"
#include "core/JtsType.h"
#include "core/CppClass.h"
#include "core/Log.h"

#include "util/ObjectOp.h"
#include "util/ScopeHelper.h"

#include <iostream>

using namespace jts;

namespace lib {
	inline void StandardLib(VM* vm) {

		env::addSymbol(vm, "nil", env::addConst(nullptr));

		// (quote value)
		env::addSymbol(vm, "quote", env::addNative([](VM* vm, Node* args) {
			return quoteObj(vm, args->val);
		}));

		// (set target value)
		env::addSymbol(vm, "set", env::addNative([](VM* vm, Node* args) {
			return env::newObj(vm, setObj(vm, args->val, evalObj(vm, args->nxt->val)));
		}));

		// (const target value)
		env::addSymbol(vm, "const", env::addNative([](VM* vm, Node* args) {
			ASSERT(args->val->initialized, "trying to make declared symbol const");

			args->val->constant = true;
			return env::newObj(vm, setObj(vm, args->val, evalObj(vm, args->nxt->val)));
		}));

		// (let ( (name value)... ) body)
		env::addSymbol(vm, "let", env::addNative([](VM* vm, Node* args) {
			BINDING_S(l, args->val->_args);

			auto bdy = args->nxt;

			while (bdy->nxt) {
				evalObj(vm, bdy->val);
				shift(&bdy);
			}

			return evalObj<true>(vm, bdy->val);
		}));

		// (fn (params) body)
		env::addSymbol(vm, "fn", env::addNative([](VM* vm, Node* args) {
			auto ret = env::newObj(vm);

			ret->spec = Spec::SYMBOL;
			ret->type = Type::JTS_FN;

			ret->_jtsFn = new JtsFn();
			ret->_jtsFn->params = args;
			ret->_jtsFn->block = args->nxt;

			return ret;
		}));

		// (defun name (params) body)
		env::addSymbol(vm, "defun", env::addNative([](VM* vm, Node* args) {
			args->val->spec = Spec::SYMBOL;
			args->val->type = Type::JTS_FN;

			args->val->_jtsFn = new JtsFn();
			args->val->_jtsFn->params = args->nxt->val->_args;
			args->val->_jtsFn->block = args->nxt->nxt;

			return env::newObj(vm, args->val);
		}));

		// (eval target)
		env::addSymbol(vm, "eval", env::addNative([](VM* vm, Node* args) {
			auto ret = evalObj(env::withEval(vm, true), args->val);
			env::withEval(vm, false);

			return ret;
		}));

		// (loop cond body)
		env::addSymbol(vm, "loop", env::addNative([](VM* vm, Node* args) {
			auto cond = args;
			Node* bdy = nullptr;
			Obj* ret = nullptr;

			while (isTrue(evalObj(vm, cond->val))) {
				bdy = args->nxt;

				while (bdy->nxt) {
					evalObj(vm, bdy->val);
					shift(&bdy);
				}

				ret = evalObj(vm, bdy->val);
			}

			return env::newObj(vm, ret);
		}));

		// (do body...)
		env::addSymbol(vm, "do", env::addNative([](VM* vm, Node* args) {
			while (args->nxt) {
				evalObj(vm, args->val);
				shift(&args);
			}

			return evalObj<true>(vm, args->val);
		}));

		// (string-int value)
		env::addSymbol(vm, "string-int", env::addNative([](VM* vm, Node* args) {
			return setTo(env::newObj(vm), std::stoi(*evalObj(vm, args->val)->_string));
		}));

		// (string-bool value)
		env::addSymbol(vm, "string-bool", env::addNative([](VM* vm, Node* args) {
			return setTo(env::newObj(vm), (bool)std::stoi(*evalObj(vm, args->val)->_string));
		}));

		// (string-float value)
		env::addSymbol(vm, "string-float", env::addNative([](VM* vm, Node* args) {
			return setTo(env::newObj(vm), std::stof(*evalObj(vm, args->val)->_string));
		}));

		// (int value)
		env::addSymbol(vm, "int", env::addNative([](VM* vm, Node* args) {
			return setTo(env::newObj(vm), castObj<jtsi>(evalObj(vm, args->val)));
		}));

		// (float value)
		env::addSymbol(vm, "float", env::addNative([](VM* vm, Node* args) {
			return setTo(env::newObj(vm), castObj<jtsf>(evalObj(vm, args->val)));
		}));

		// (char value)
		env::addSymbol(vm, "char", env::addNative([](VM* vm, Node* args) {
			return setTo(env::newObj(vm), castObj<jtsc>(evalObj(vm, args->val)));
		}));

		// (bool value)
		env::addSymbol(vm, "bool", env::addNative([](VM* vm, Node* args) {
			return setTo(env::newObj(vm), castObj<jtsb>(evalObj(vm, args->val)));
		}));

		// (string value)
		env::addSymbol(vm, "string", env::addNative([](VM* vm, Node* args) {
			return setTo(env::newObj(vm), new str(toString(evalObj(vm, args->val))));
		}));

		// (input) --> (set in (input))
		env::addSymbol(vm, "input", env::addNative([](VM* vm, Node* args) {
			str* input = new str();

			std::getline(std::cin, *input);

			return setTo(env::newObj(vm), input);
		}));

		// (print args)
		env::addSymbol(vm, "print", env::addNative([](VM* vm, Node* args) {
			while (args->nxt) {
				printObj(evalObj(vm, args->val), false);
				args = args->nxt;
			}

			return printObj(evalObj<true>(vm, args->val), false);
		}));

		// (println args)
		env::addSymbol(vm, "println", env::addNative([](VM* vm, Node* args) {
			while (args->nxt) {
				printObj(evalObj(vm, args->val), false);
				args = args->nxt;
			}

			return printObj(evalObj<true>(vm, args->val), true);
		}));
	}
}

#endif