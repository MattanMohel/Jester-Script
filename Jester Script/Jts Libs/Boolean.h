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
		env::addSymbol(vm, "if", env::addNative([](Obj* ret, ObjNode* args, bool eval)
		{
			if (isTrue(evalObj(args->value, eval))) {
				binaryOp<Binary::SET>(ret, evalObj(args->next->value, eval));
			}
			else {
				binaryOp<Binary::SET>(ret, evalObj(args->next->next->value, eval));
			}
		}));

		// (match value case 1 if-true... case n if-true)
		env::addSymbol(vm, "match", env::addNative([](Obj* ret, ObjNode* args, bool eval)
		{
			Obj* val = evalObj(args->value, eval);

			auto* pattern = args->next;

			while (true) {
				if (isEqual(val, evalObj(pattern->value, eval))) {
					evalObj(pattern->next->value, eval);
					break;
				}
				else if (!pattern->next) {
					binaryOp<Binary::SET>(ret, NIL);
					break;
				}

				pattern = pattern->next->next;
			}
		}));

		env::addSymbol(vm, "when", env::addNative([](Obj* ret, ObjNode* args, bool eval)
		{
			if (isTrue(evalObj(args->value, eval))) {
				binaryOp<Binary::SET>(ret, evalObj(args->next->value, eval));
			}
			else {
				binaryOp<Binary::SET>(ret, NIL);
			}
		}));

		env::addSymbol(vm, "unless", env::addNative([](Obj* ret, ObjNode* args, bool eval)
		{
			if (!isTrue(evalObj(args->value, eval))) {
				binaryOp<Binary::SET>(ret, evalObj(args->next->value, eval));
			}
			else {
				binaryOp<Binary::SET>(ret, NIL);
			}
		}));

		env::addSymbol(vm, "=", env::addNative([](Obj* ret, ObjNode* args, bool eval)
		{
			setTo<bool>(ret, isEqual(evalObj(args->value, eval), evalObj(args->next->value, eval)));
		}));

		env::addSymbol(vm, ">", env::addNative([](Obj* ret, ObjNode* args, bool eval)
		{
			setTo<bool>(ret, isGreater(evalObj(args->value, eval), evalObj(args->next->value, eval)));
		}));

		env::addSymbol(vm, ">=", env::addNative([](Obj* ret, ObjNode* args, bool eval)
		{
			setTo<bool>(ret, isGreaterEq(evalObj(args->value, eval), evalObj(args->next->value, eval)));
		}));

		env::addSymbol(vm, "<", env::addNative([](Obj* ret, ObjNode* args, bool eval)
		{
			setTo<bool>(ret, !isGreaterEq(evalObj(args->value, eval), evalObj(args->next->value, eval)));
		}));

		env::addSymbol(vm, "<=", env::addNative([](Obj* ret, ObjNode* args, bool eval)
		{
			setTo<bool>(ret, !isGreater(evalObj(args->value, eval), evalObj(args->next->value, eval)));
		}));

		env::addSymbol(vm, "not", env::addNative([](Obj* ret, ObjNode* args, bool eval)
		{
			setTo<bool>(ret, !isTrue(evalObj(args->value, eval)));
		}));

		env::addSymbol(vm, "and", env::addNative([](Obj* ret, ObjNode* args, bool eval)
		{
			bool state = false;

			while (args) {
				state = isTrue(evalObj(args->value, eval));

				if (!state) {
					break;
				}

				args = args->next;
			}

			setTo<bool>(ret, state);
		}));

		env::addSymbol(vm, "or", env::addNative([](Obj* ret, ObjNode* args, bool eval)
		{
			bool state = false;

			while (args) {
				state = isTrue(evalObj(args->value, eval));

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