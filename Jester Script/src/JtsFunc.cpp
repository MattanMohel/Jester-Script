#include "JtsFunc.h"
#include "Execute.h"
#include "Object.h"
#include "Operations.h"
#include "VM.h"

namespace jts
{
	Obj* Func::Call (ObjNode* args, bool eval)
	{
		ObjNode* paramCpy = nullptr; 
		auto* paramPtr = params->value->_args;
		args = args->next;

		if (params)
		{
			/*
				Assign passed parameter values to the parameter-copy

				1) Assigned copy-values are copied to intermediary object 'value'
				2) copy-values assigned to the preliminary param values
				3) param values assigned to intermediary object

				- Copy reverts the parameters to their previous state upon 
				  termination of function which allows for recursion

				EX:
					
					(defn fac (n) 
						(if (= n 0) 
							1 
							(* n (fac (- n 1)))))

					(fac 5)

					n's values go from 5 -> 4 -> 3 -> 2 -> 1 -> 0
					
					- once end condition is met, the recursive call 'recoils' to return (* 1 1 2 3 4 5)
					- n has to reset to the previous parameter value so it doesn't return (* 1 1 1 1 1 1)
					- n recoils back then 1 -> 1 -> 2 -> 3 -> 4 -> 5 -> nil (original parameter state in nil) 
			*/

			paramCpy = env::AcquireNode();
			auto* paramCpyPtr = paramCpy;

			// assign copy to passed values

			while (true)
			{
				BinaryOp<Binary::SET>(paramCpyPtr->value, EvalObj(args, eval));

				args = args->next;

				if (args)
				{
					paramCpyPtr->next = env::AcquireNode();
					paramCpyPtr = paramCpyPtr->next;
					continue;
				}

				break;
			}

			paramCpyPtr = paramCpy;

			// assign params to passed values and copy to original values

			while (paramCpyPtr)
			{
				Obj* value = env::glbl_objPool.acquire();

				BinaryOp<Binary::SET>(value, paramCpyPtr->value);
				BinaryOp<Binary::SET>(paramCpyPtr->value, paramPtr->value);
				BinaryOp<Binary::SET>(paramPtr->value, value);

				paramCpyPtr = paramCpyPtr->next;
				paramPtr = paramPtr->next;
			}
		}

		// Execute function body

		auto* block = codeBlock;

		while (block->next)
		{
			EvalObj(block, eval);
			block = block->next;
		}

		// Evaluate return

		Obj* ret = env::glbl_objPool.peek();
		BinaryOp<Binary::SET>(ret, EvalObj(block, eval));

		// Reset parameters

		paramCpy = paramCpy;
		paramPtr = params->value->_args;

		while (paramCpy)
		{
			BinaryOp<Binary::SET>(paramPtr->value, paramCpy->value);
			
			env::ReleaseNode(paramCpy);
			paramCpy = paramCpy->next;

			paramPtr = paramPtr->next;
		}

		return ret;
	}
}