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
		env::AddNative(vm, "if", [](ObjNode* in) -> Obj*
		{
			if (isTrue(in->args))
			{
				return EvalObj(in->args->next);
			}

			return EvalObj(in->args->next->next);
		});

		env::AddNative(vm, "equal", [](ObjNode* in) -> Obj*
		{
			return SetState(in, isEqual(in->args, in->args->next));
		});				
		
		env::AddNative(vm, "not", [](ObjNode* in) -> Obj*
		{
			return SetState(in, !isTrue(in->args));
		});		
		
		env::AddNative(vm, "and", [](ObjNode* in) -> Obj*
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
		});		
		
		env::AddNative(vm, "or", [](ObjNode* in) -> Obj*
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
		});
	}
}

#endif