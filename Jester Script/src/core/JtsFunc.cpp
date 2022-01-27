#include "JtsFunc.h"
#include "Execute.h"
#include "Object.h"
#include "VM.h"

#include "util/ObjectOp.h"
#include "util/ListOp.h"

namespace jts {

	Obj* JtsFn::call(VM* vm, Node* args) {

		Node* prvVal = env::bindEnv(vm, params, lst::eval(vm, args));
		
		Node* blockPtr = block;

		while (blockPtr->nxt) {
			evalObj(vm, blockPtr->val);
			shift(&blockPtr);
		}

		Obj* ret = env::newObj(vm, evalObj(vm, blockPtr->val));

		env::unbindEnv(vm, params, prvVal);

		return ret;
	}
}