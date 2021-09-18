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
		env::AddSymbol(vm, "if", env::AddNative([](ObjNode* in) -> Obj*
		{
			if (isTrue(in->args))
			{
				return EvalObj(in->args->next);
			}

			return EvalObj(in->args->next->next);
		}));

		env::AddSymbol(vm, "equal", env::AddNative([](ObjNode* in) -> Obj*
		{
			return SetState(in, isEqual(in->args, in->args->next));
		}));			
		
		env::AddSymbol(vm, ">", env::AddNative([](ObjNode* in) -> Obj*
		{
			return SetState(in, isGreater(in->args, in->args->next));
		}));			
		
		env::AddSymbol(vm, ">=", env::AddNative([](ObjNode* in) -> Obj*
		{
			return SetState(in, isGreaterEq(in->args, in->args->next));
		}));				
		
		env::AddSymbol(vm, "<", env::AddNative([](ObjNode* in) -> Obj*
		{
			return SetState(in, !isGreaterEq(in->args, in->args->next));
		}));				
		
		env::AddSymbol(vm, "<=", env::AddNative([](ObjNode* in) -> Obj*
		{
			return SetState(in, !isGreater(in->args, in->args->next));
		}));				
		
		env::AddSymbol(vm, "not", env::AddNative([](ObjNode* in) -> Obj*
		{
			return SetState(in, !isTrue(in->args));
		}));		
		
		env::AddSymbol(vm, "and", env::AddNative([](ObjNode* in) -> Obj*
		{
			auto* rest = in->args;
			bool state = false;

			while (rest)
			{
				state = isTrue(rest);

				if (!state)
				{
					break;
				}

				rest = rest->next;
			}

			return SetState(in, state);
		}));		
		
		env::AddSymbol(vm, "or", env::AddNative([](ObjNode* in) -> Obj*
		{
			auto* rest = in->args;
			bool state = false;

			while (rest)
			{
				state = isTrue(rest);

				if (state)
				{
					break;
				}

				rest = rest->next;
			}

			return SetState(in, state);
		}));
	}
}

#endif