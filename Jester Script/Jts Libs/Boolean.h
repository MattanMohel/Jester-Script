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
		env::addSymbol(vm, "if", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			if (isTrue(evalObj(args->value, eval))) {
				return evalObj(args->next->value, eval);
			}

			return evalObj(args->next->next->value, eval);
		}));

		// (match value case 1 if-true... case n if-true)
		env::addSymbol(vm, "match", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			Obj* val = evalObj(args->value, eval);

			auto* pattern = args->next;

			while (true) {
				if (isEqual(val, evalObj(pattern->value, eval))) {
					return evalObj(pattern->next->value, eval);
				}

				if (!pattern->next) return NIL;

				pattern = pattern->next->next;
			}
		}));

		env::addSymbol(vm, "when", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			if (isTrue(evalObj(args->value, eval))) {
				return evalObj(args->next->value, eval);
			}

			return NIL;
		}));

		env::addSymbol(vm, "unless", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			if (!isTrue(evalObj(args->value, eval))) {
				return evalObj(args->next->value, eval);
			}

			return NIL;
		}));

		env::addSymbol(vm, "=", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			return setTo(ret, isEqual(evalObj(args->value, eval), evalObj(args->next->value, eval)));
		}));

		env::addSymbol(vm, ">", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			return setTo(ret, isGreater(evalObj(args->value, eval), evalObj(args->next->value, eval)));
		}));

		env::addSymbol(vm, ">=", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			return setTo(ret, isGreaterEq(evalObj(args->value, eval), evalObj(args->next->value, eval)));
		}));

		env::addSymbol(vm, "<", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			return setTo(ret, !isGreaterEq(evalObj(args->value, eval), evalObj(args->next->value, eval)));
		}));

		env::addSymbol(vm, "<=", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			return setTo(ret, !isGreater(evalObj(args->value, eval), evalObj(args->next->value, eval)));
		}));

		env::addSymbol(vm, "not", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			return setTo(ret, !isTrue(evalObj(args->value, eval)));
		}));

		env::addSymbol(vm, "and", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			bool state = false;

			while (args) {
				state = isTrue(evalObj(args->value, eval));

				if (!state) {
					break;
				}

				args = args->next;
			}

			return setTo(ret, state);
		}));

		env::addSymbol(vm, "or", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			bool state = false;

			while (args) {
				state = isTrue(evalObj(args->value, eval));

				if (state) {
					break;
				}

				args = args->next;
			}

			return setTo(ret, state);
		}));
	}
}

#endif