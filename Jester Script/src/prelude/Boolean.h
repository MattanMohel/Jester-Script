#ifndef BOOLEAN_H
#define BOOLEAN_H

#include "core/Types.h"
#include "core/Object.h"
#include "core/Execute.h"
#include "core/VM.h"

#include "util/ScopeHelper.h"
#include "util/ObjectOp.h"
#include "util/Pool.h"

using namespace jts;

namespace lib {
	inline void BooleanLib(VM* vm) {

		env::addSymbol(vm, "T", env::addConst<bool>(true));

		env::addSymbol(vm, "F", env::addConst<bool>(false));

		// (if cond if-true else)
		env::addSymbol(vm, "if", env::addNative([](VM* vm, Node* args) {
			if (isTrue(evalObj(vm, args->val))) {
				return vm, evalObj<true>(vm, args->nxt->val);
			}

			return evalObj<true>(vm, args->nxt->nxt->val);
		}));

		// (when cond if-true)
		env::addSymbol(vm, "when", env::addNative([](VM* vm, Node* args) {
			if (isTrue(evalObj(vm, args->val))) {
				return evalObj<true>(vm, args->nxt->val);
			}

			return NIL;
		}));

		// (match value case-1 if-true... case-n if-true _ default)
		env::addSymbol(vm, "match", env::addNative([](VM* vm, Node* args) {
			auto val = evalObj<true>(vm, args->val);
			auto pat = args->nxt;

			while (pat->nxt->nxt && !isEqual(val, evalObj(vm, pat->val))) {
				pat = pat->nxt->nxt;
			}

			env::releaseObj(vm, val);

			return evalObj<true>(vm, pat->nxt->val);
		}));

		env::addSymbol(vm, "=", env::addNative([](VM* vm, Node* args) {
			WRAPPER_S(vls, evalObj<true>(vm, args->val), evalObj<true>(vm, args->nxt->val));
			return setTo(env::newObj(vm), isEqual(vls.get(0), vls.get(1)));
		}));

		env::addSymbol(vm, ">", env::addNative([](VM* vm, Node* args) {
			WRAPPER_S(vls, evalObj<true>(vm, args->val), evalObj<true>(vm, args->nxt->val));
			return setTo(env::newObj(vm), isGreater(vls.get(0), vls.get(1)));
		}));

		env::addSymbol(vm, ">=", env::addNative([](VM* vm, Node* args) {
			WRAPPER_S(vls, evalObj<true>(vm, args->val), evalObj<true>(vm, args->nxt->val));
			return setTo(env::newObj(vm), isGreaterEq(vls.get(0), vls.get(1)));
		}));

		env::addSymbol(vm, "<", env::addNative([](VM* vm, Node* args) {
			WRAPPER_S(vls, evalObj<true>(vm, args->val), evalObj<true>(vm, args->nxt->val));
			return setTo(env::newObj(vm), !isGreaterEq(vls.get(0), vls.get(1)));
		}));

		env::addSymbol(vm, "<=", env::addNative([](VM* vm, Node* args) {
			WRAPPER_S(vls, evalObj<true>(vm, args->val), evalObj<true>(vm, args->nxt->val));
			return setTo(env::newObj(vm), !isGreater(vls.get(0), vls.get(1)));
		}));

		env::addSymbol(vm, "not", env::addNative([](VM* vm, Node* args) {
			return setTo(env::newObj(vm), !isTrue(evalObj(vm, args->val)));
		}));

		env::addSymbol(vm, "and", env::addNative([](VM* vm, Node* args) {
			bool state = false;

			while (state && args) {
				state = isTrue(evalObj(vm, args->val));
				args = args->nxt;
			}

			return setTo<bool>(env::newObj(vm), state);
		}));

		env::addSymbol(vm, "or", env::addNative([](VM* vm, Node* args) {
			bool state = false;

			while (state && args) {
				state = isTrue(evalObj(vm, args->val));
				args = args->nxt;
			}

			return setTo<bool>(env::newObj(vm), state);
		}));
	}
}

#endif