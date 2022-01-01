#ifndef BOOLEAN_H
#define BOOLEAN_H

#include "../src/Types.h"
#include "../src/Object.h"
#include "../src/Operations.h"
#include "../src/Execute.h"
#include "../src/VM.h"

using namespace jts;

namespace lib {
	inline void BooleanLib(VM* vm) {

		env::addSymbol(vm, "T", env::addConst<bool>(true));

		env::addSymbol(vm, "F", env::addConst<bool>(false));

		// (if cond if-true else)
		env::addSymbol(vm, "if", env::addNative([](VM* vm, Obj* ret, ObjNode* args, bool eval)
		{
			if (isTrue(evalObj(vm, args->value, eval))) {
				set(vm, ret, evalObj(vm, args->next->value, eval));
			}
			else {
				set(vm, ret, evalObj(vm, args->next->next->value, eval));
			}
		}));

		// (match value case 1 if-true... case n if-true)
		env::addSymbol(vm, "match", env::addNative([](VM* vm, Obj* ret, ObjNode* args, bool eval)
		{
			Obj* val = evalObj(vm, args->value, eval);

			auto* pattern = args->next;

			while (true) {
				if (isEqual(val, evalObj(vm, pattern->value, eval))) {
					evalObj(vm, pattern->next->value, eval);
					break;
				}
				else if (!pattern->next) {
					set(vm, ret, NIL);
					break;
				}

				pattern = pattern->next->next;
			}
		}));

		env::addSymbol(vm, "when", env::addNative([](VM* vm, Obj* ret, ObjNode* args, bool eval)
		{
			if (isTrue(evalObj(vm, args->value, eval))) {
				set(vm, ret, evalObj(vm, args->next->value, eval));
			}
			else {
				set(vm, ret, NIL);
			}
		}));

		env::addSymbol(vm, "unless", env::addNative([](VM* vm, Obj* ret, ObjNode* args, bool eval)
		{
			if (!isTrue(evalObj(vm, args->value, eval))) {
				set(vm, ret, evalObj(vm, args->next->value, eval));
			}
			else {
				set(vm, ret, NIL);
			}
		}));

		env::addSymbol(vm, "=", env::addNative([](VM* vm, Obj* ret, ObjNode* args, bool eval)
		{
			setTo<bool>(ret, isEqual(evalObj(vm, args->value, eval), evalObj(vm, args->next->value, eval)));
		}));

		env::addSymbol(vm, ">", env::addNative([](VM* vm, Obj* ret, ObjNode* args, bool eval)
		{
			setTo<bool>(ret, isGreater(evalObj(vm, args->value, eval), evalObj(vm, args->next->value, eval)));
		}));

		env::addSymbol(vm, ">=", env::addNative([](VM* vm, Obj* ret, ObjNode* args, bool eval)
		{
			setTo<bool>(ret, isGreaterEq(evalObj(vm, args->value, eval), evalObj(vm, args->next->value, eval)));
		}));

		env::addSymbol(vm, "<", env::addNative([](VM* vm, Obj* ret, ObjNode* args, bool eval)
		{
			setTo<bool>(ret, !isGreaterEq(evalObj(vm, args->value, eval), evalObj(vm, args->next->value, eval)));
		}));

		env::addSymbol(vm, "<=", env::addNative([](VM* vm, Obj* ret, ObjNode* args, bool eval)
		{
			setTo<bool>(ret, !isGreater(evalObj(vm, args->value, eval), evalObj(vm, args->next->value, eval)));
		}));

		env::addSymbol(vm, "not", env::addNative([](VM* vm, Obj* ret, ObjNode* args, bool eval)
		{
			setTo<bool>(ret, !isTrue(evalObj(vm, args->value, eval)));
		}));

		env::addSymbol(vm, "and", env::addNative([](VM* vm, Obj* ret, ObjNode* args, bool eval)
		{
			bool state = false;

			while (args) {
				state = isTrue(evalObj(vm, args->value, eval));

				if (!state) {
					break;
				}

				args = args->next;
			}

			setTo<bool>(ret, state);
		}));

		env::addSymbol(vm, "or", env::addNative([](VM* vm, Obj* ret, ObjNode* args, bool eval)
		{
			bool state = false;

			while (args) {
				state = isTrue(evalObj(vm, args->value, eval));

				if (state) {
					break;
				}

				args = args->next;
			}

			setTo<bool>(ret, state);
		}));
	}
}

#endif