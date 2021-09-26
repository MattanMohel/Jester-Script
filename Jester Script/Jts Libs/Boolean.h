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
		env::AddSymbol(vm, "if", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			if (isTrue(EvalObj(args, eval)))
			{
				return EvalObj(args->next, eval);
			}

			return EvalObj(args->next->next, eval);
		}));

		env::AddSymbol(vm, "equal", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			return SetState(ret, isEqual(EvalObj(args, eval), EvalObj(args->next, eval)));
		}));			
		
		env::AddSymbol(vm, ">", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			return SetState(ret, isGreater(EvalObj(args, eval), EvalObj(args->next, eval)));
		}));			
		
		env::AddSymbol(vm, ">=", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			return SetState(ret, isGreaterEq(EvalObj(args, eval), EvalObj(args->next, eval)));
		}));				
		
		env::AddSymbol(vm, "<", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			return SetState(ret, !isGreaterEq(EvalObj(args, eval), EvalObj(args->next, eval)));
		}));				
		
		env::AddSymbol(vm, "<=", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			return SetState(ret, !isGreater(EvalObj(args, eval), EvalObj(args->next, eval)));
		}));				
		
		env::AddSymbol(vm, "not", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			return SetState(ret, !isTrue(EvalObj(args, eval)));
		}));		
		
		env::AddSymbol(vm, "and", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			bool state = false;

			while (args)
			{
				state = isTrue(EvalObj(args, eval));

				if (!state)
				{
					break;
				}

				args = args->next;
			}

			return SetState(ret, state);
		}));		
		
		env::AddSymbol(vm, "or", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			bool state = false;

			while (args)
			{
				state = isTrue(EvalObj(args, eval));

				if (state)
				{
					break;
				}

				args = args->next;
			}

			return SetState(ret, state);
		}));
	}
}

#endif