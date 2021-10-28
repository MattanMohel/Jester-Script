#include "JtsFunc.h"
#include "Execute.h"
#include "Object.h"
#include "Operations.h"
#include "VM.h"

namespace jts
{
	Obj* JtsFn::call (ObjNode* args, bool eval)
	{
		ObjNode* paramCpy = nullptr; 
		auto* paramPtr = params->value->_args;

		if (params->value->_args && args)
		{
			/*
				1) Assigns passed parameter values to the parameter-copy
				2) Assigned copy-values are copied to intermediary object 'value'
				3) copy-values assigned to the preliminary param values
				4) param values assigned to intermediary object

				- Copy reverts the parameters to their previous state upon 
				  termination of function which allows for recursion

				EX:
					
					(defn fac (n) 
						(if (= n 0) 
							1 
							(* n (fac (- n 1)))))

					(fac 5)

					n's values go from 5 -> 4 -> 3 -> 2 -> 1 -> 0
					
					once end condition is met, the recursive call 'recoils' to return (* 1 1 2 3 4 5)
					n has to reset to the previous parameter value so it doesn't return (* 1 1 1 1 1 1)
					n then 'recoils' back 1 -> 1 -> 2 -> 3 -> 4 -> 5 -> nil (original velue) 
			*/

			paramCpy = env::acquireNode();
			auto* paramCpyPtr = paramCpy;

			// assign copy to passed values

			while (true)
			{
				binaryOp<Binary::SET>(paramCpyPtr->value, evalObj(args->value, eval));

				args = args->next;

				if (args)
				{
					paramCpyPtr->next = env::acquireNode();
					paramCpyPtr = paramCpyPtr->next;
					continue;
				}

				break;
			}

			paramCpyPtr = paramCpy;

			// assign params to passed values and copy to original values

			while (paramCpyPtr)
			{
				Obj* value = env::glbl_objPool.peek();

				binaryOp<Binary::SET>(value, paramCpyPtr->value);
				binaryOp<Binary::SET>(paramCpyPtr->value, paramPtr->value);
				binaryOp<Binary::SET>(paramPtr->value, value);

				paramCpyPtr = paramCpyPtr->next;
				paramPtr = paramPtr->next;
			}
		}

		// Execute function body

		auto* block = codeBlock;

		while (block->next)
		{
			evalObj(block->value, eval);
			block = block->next;
		}

		// Evaluate return

		Obj* ret = env::glbl_objPool.peek();
		binaryOp<Binary::SET>(ret, evalObj(block->value, eval));

		// Reset parameters

		paramCpy = paramCpy;
		paramPtr = params->value->_args;

		while (paramCpy)
		{
			binaryOp<Binary::SET>(paramPtr->value, paramCpy->value);
			
			env::releaseNode(paramCpy);
			paramCpy = paramCpy->next;

			paramPtr = paramPtr->next;
		}

		return ret;
	}
}