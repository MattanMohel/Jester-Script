#include "JtsFunc.h"
#include "Execute.h"
#include "Object.h"
#include "Operations.h"
#include "VM.h"

namespace jts {

	Obj* JtsFn::call(ObjNode* args, bool eval) {

		auto paramPtr = params->value->_args;
		auto paramValPtr = args;
		
		// store a copy of previous parameter values
		auto prevParamPtr = listCopy(params->value->_args, 
			[&eval](Obj* obj) {
				Obj* ret = env::glbl_objPool.acquire();
				binaryOp<Binary::SET>(ret, evalObj(obj, eval));
				return ret;
		});
		
		if (params->value->_args && args) {
			while (paramPtr) {
				binaryOp<Binary::SET>(paramPtr->value, evalObj(paramValPtr->value, eval));

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

		// Reset parameters to previous values

		paramPtr = params->value->_args;

		while (paramPtr) {
			binaryOp<Binary::SET>(paramPtr->value, prevParamPtr->value);

			env::releaseNode(prevParamPtr);

			prevParamPtr = prevParamPtr->next;
			paramPtr = paramPtr->next;
		}

		return ret;
	}
}