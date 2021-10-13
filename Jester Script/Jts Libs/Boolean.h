#ifndef BOOLEAN_H
#define BOOLEAN_H

#include "../src/Object.h"
#include "../src/Operations.h"
#include "../src/Execute.h"
#include "../src/VM.h"

using namespace jts;

namespace lib
{
	inline void BooleanLib(VM* vm)
	{
		env::AddSymbol(vm, "T", env::AddConst<bool>(true));

		env::AddSymbol(vm, "F", env::AddConst<bool>(false));

		// (if cond if-true else)
		env::AddSymbol(vm, "if", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			if (isTrue(EvalObj(args->value, eval)))
			{
				return EvalObj(args->next->value, eval);
			}

			return EvalObj(args->next->next->value, eval);
		}));			
		
		// (match value case 1 if-true... case n if-true)
		env::AddSymbol(vm, "match", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			Obj* val = EvalObj(args->value, eval);

			auto* curCase = args->next;

			while (true)
			{
				if (isEqual(val, EvalObj(curCase->value, eval)))
				{
					return EvalObj(curCase->next->value, eval);
				}

				if (!curCase->next) return NIL;

				curCase = curCase->next->next;
			}
		}));		
		
		env::AddSymbol(vm, "when", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			if (isTrue(EvalObj(args->value, eval)))
			{
				return EvalObj(args->next->value, eval);
			}

			return NIL;
		}));		
		
		env::AddSymbol(vm, "unless", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			if (!isTrue(EvalObj(args->value, eval)))
			{
				return EvalObj(args->next->value, eval);
			}

			return NIL;
		}));

		env::AddSymbol(vm, "=", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			return SetTo(ret, isEqual(EvalObj(args->value, eval), EvalObj(args->next->value, eval)));
		}));			
		
		env::AddSymbol(vm, ">", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			return SetTo(ret, isGreater(EvalObj(args->value, eval), EvalObj(args->next->value, eval)));
		}));			
		
		env::AddSymbol(vm, ">=", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			return SetTo(ret, isGreaterEq(EvalObj(args->value, eval), EvalObj(args->next->value, eval)));
		}));				
		
		env::AddSymbol(vm, "<", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			return SetTo(ret, !isGreaterEq(EvalObj(args->value, eval), EvalObj(args->next->value, eval)));
		}));				
		
		env::AddSymbol(vm, "<=", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			return SetTo(ret, !isGreater(EvalObj(args->value, eval), EvalObj(args->next->value, eval)));
		}));				
		
		env::AddSymbol(vm, "not", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			return SetTo(ret, !isTrue(EvalObj(args->value, eval)));
		}));		
		
		env::AddSymbol(vm, "and", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			bool state = false;

			while (args)
			{
				state = isTrue(EvalObj(args->value, eval));

				if (!state)
				{
					break;
				}

				args = args->next;
			}

			return SetTo(ret, state);
		}));		
		
		env::AddSymbol(vm, "or", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			bool state = false;

			while (args)
			{
				state = isTrue(EvalObj(args->value, eval));

				if (state)
				{
					break;
				}

				args = args->next;
			}

			return SetTo(ret, state);
		}));
	}
}

#endif