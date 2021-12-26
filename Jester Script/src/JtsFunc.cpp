#include "JtsFunc.h"
#include "Execute.h"
#include "Object.h"
#include "Operations.h"
#include "VM.h"

namespace jts {

	Obj* JtsFn::call(ObjNode* args, bool eval) {

		auto paramPtr = params->value->_args;
		auto paramValPtr = args;
		
		auto prevParamPtr = copyList(params->value->_args)->_args;
		

		if (params->value->_args && args) {
			
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

			// assign params to passed values and copy to original values -
			// actual params now point to their passed value and paramValPtr 
			// stores their previous value

			while (paramPtr) {
				binaryOp<Binary::SET>(paramPtr->value, paramValPtr->value);

				paramValPtr = paramValPtr->next;
				paramPtr = paramPtr->next;
			}
		}

		// Execute function body

		auto block = codeBlock;

		while (block->next) {
			evalObj(block->value, eval);
			block = block->next;
		}

		// Evaluate return

		Obj* ret = env::glbl_objPool.peek();
		binaryOp<Binary::SET>(ret, evalObj(block->value, eval));

		// Reset parameters

		paramPtr = params->value->_args;

		while (paramPtr) {
			binaryOp<Binary::SET>(paramPtr->value, prevParamPtr->value);

			prevParamPtr = prevParamPtr->next;
			paramPtr = paramPtr->next;
		}

		return ret;
	}
}