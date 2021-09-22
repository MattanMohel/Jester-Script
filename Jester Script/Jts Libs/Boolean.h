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
		env::AddSymbol(vm, "if", env::AddNative([](ObjNode* fn, ObjNode* args) -> Obj*
		{
			if (isTrue(args))
			{
				return EvalObj(args->next);
			}

			return EvalObj(args->next->next);
		}));

		env::AddSymbol(vm, "equal", env::AddNative([](ObjNode* fn, ObjNode* args) -> Obj*
		{
			return SetState(fn, isEqual(args, args->next));
		}));			
		
		env::AddSymbol(vm, ">", env::AddNative([](ObjNode* fn, ObjNode* args) -> Obj*
		{
			return SetState(fn, isGreater(args, args->next));
		}));			
		
		env::AddSymbol(vm, ">=", env::AddNative([](ObjNode* fn, ObjNode* args) -> Obj*
		{
			return SetState(fn, isGreaterEq(args, args->next));
		}));				
		
		env::AddSymbol(vm, "<", env::AddNative([](ObjNode* fn, ObjNode* args) -> Obj*
		{
			return SetState(fn, !isGreaterEq(args, args->next));
		}));				
		
		env::AddSymbol(vm, "<=", env::AddNative([](ObjNode* fn, ObjNode* args) -> Obj*
		{
			return SetState(fn, !isGreater(args, args->next));
		}));				
		
		env::AddSymbol(vm, "not", env::AddNative([](ObjNode* fn, ObjNode* args) -> Obj*
		{
			return SetState(fn, !isTrue(args));
		}));		
		
		env::AddSymbol(vm, "and", env::AddNative([](ObjNode* fn, ObjNode* args) -> Obj*
		{
			bool state = false;

			while (args)
			{
				state = isTrue(args);

				if (!state)
				{
					break;
				}

				args = args->next;
			}

			return SetState(fn, state);
		}));		
		
		env::AddSymbol(vm, "or", env::AddNative([](ObjNode* fn, ObjNode* args) -> Obj*
		{
			bool state = false;

			while (args)
			{
				state = isTrue(args);

				if (state)
				{
					break;
				}

				args = args->next;
			}

			return SetState(fn, state);
		}));
	}
}

#endif