#ifndef BOOLEAN_H
#define BOOLEAN_H

#include "../src/Types.h"
#include "../src/Object.h"
#include "../src/Operations.h"
#include "../src/Execute.h"
#include "../src/VM.h"

#include "../utils/Pool.h"

using namespace jts;

namespace lib {
	inline void BooleanLib(VM* vm) {

		env::addSymbol(vm, "T", env::addConst<bool>(true));

		env::addSymbol(vm, "F", env::addConst<bool>(false));

		// (if cond if-true else)
		env::addSymbol(vm, "if", env::addNative([](VM* vm, Node* args)
		{
			if (isTrue(evalObj(vm, args->val))) {
				return env::newObj(vm, evalObj(vm, args->nxt->val));
			}
			
			return env::newObj(vm, evalObj(vm, args->nxt->nxt->val));
		}));

		// (match value case-1 if-true... case-n if-true _ default)
		env::addSymbol(vm, "match", env::addNative([](VM* vm, Node* args)
		{
			auto val = evalObj(vm, args->val);
			auto pat = args->nxt;

			while (pat->nxt && !isEqual(val, evalObj(vm, pat->val))) {
				pat = pat->nxt->nxt;
			}

			return env::newObj(vm, evalObj(vm, pat->nxt->val));
		}));

		env::addSymbol(vm, "=", env::addNative([](VM* vm, Node* args)
		{
			return setTo<bool>(env::newObj(vm), 
				isEqual(evalObj(vm, args->val), evalObj(vm, args->nxt->val)));
		}));

		env::addSymbol(vm, ">", env::addNative([](VM* vm, Node* args)
		{
			return setTo<bool>(env::newObj(vm), 
				isGreater(evalObj(vm, args->val), evalObj(vm, args->nxt->val)));
		}));

		env::addSymbol(vm, ">=", env::addNative([](VM* vm, Node* args)
		{
			return setTo<bool>(env::newObj(vm), 
				isGreaterEq(evalObj(vm, args->val), evalObj(vm, args->nxt->val)));
		}));

		env::addSymbol(vm, "<", env::addNative([](VM* vm, Node* args)
		{
			return setTo<bool>(env::newObj(vm), 
				!isGreaterEq(evalObj(vm, args->val), evalObj(vm, args->nxt->val)));
		}));

		env::addSymbol(vm, "<=", env::addNative([](VM* vm, Node* args)
		{
			return setTo<bool>(env::newObj(vm), 
				!isGreater(evalObj(vm, args->val), evalObj(vm, args->nxt->val)));
		}));

		env::addSymbol(vm, "not", env::addNative([](VM* vm, Node* args)
		{
			return setTo<bool>(env::newObj(vm), 
				!isTrue(evalObj(vm, args->val)));
		}));

		env::addSymbol(vm, "and", env::addNative([](VM* vm, Node* args)
		{
			bool state = false;

			while (state && args) {
				state = isTrue(evalObj(vm, args->val));
				args = args->nxt;
			}

			return setTo<bool>(env::newObj(vm), state);
		}));

		env::addSymbol(vm, "or", env::addNative([](VM* vm, Node* args)
		{
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