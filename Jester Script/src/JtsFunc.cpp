#include "JtsFunc.h"
#include "Execute.h"
#include "Object.h"
#include "Operations.h"
#include "VM.h"

namespace jts {

	Obj* JtsFn::call(VM* vm, ObjNode* args, bool eval) {

		auto paramPtr = params->value->_args;
		auto paramValPtr = args;
		
		// store a copy of previous parameter values
		auto prevParamPtr = listCopy(vm, params->value->_args, 
			[&eval](VM* vm, Obj* obj) {
				Obj* ret = vm->objPool->acquire();
				set(vm, ret, evalObj(vm, obj, eval));
				return ret;
		});
		
		if (params->value->_args && args) {
			while (paramPtr) {
				set(vm, paramPtr->value, evalObj(vm, paramValPtr->value, eval));

				paramValPtr = paramValPtr->next;
				paramPtr = paramPtr->next;
			}
		}

		// Execute function body

		auto block = codeBlock;

		while (block->next) {
			evalObj(vm, block->value, eval);
			block = block->next;
		}

		// Evaluate return

		Obj* ret = vm->objPool->peek();
		set(vm, ret, evalObj(vm, block->value, eval));

		// Reset parameters to previous values

		paramPtr = params->value->_args;

		while (paramPtr) {
			set(vm, paramPtr->value, prevParamPtr->value);

			env::releaseNode(vm, prevParamPtr);

			prevParamPtr = prevParamPtr->next;
			paramPtr = paramPtr->next;
		}

		return ret;
	}
}