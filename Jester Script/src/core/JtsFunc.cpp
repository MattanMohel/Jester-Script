#include "JtsFunc.h"
#include "Execute.h"
#include "Object.h"
#include "VM.h"

#include "util/ObjectOp.h"

namespace jts {

	Obj* JtsFn::call(VM* vm, Node* args) {

		Node* prvVal = env::pushEnv(vm, params, args);
		
		Node* blockPtr = block;

		while (blockPtr->nxt) {
			evalObj(vm, blockPtr->val);
			shift(&blockPtr);
		}

		Obj* ret = setObj(vm, env::newObj(vm), evalObj(vm, blockPtr->val));

		env::endEnv(vm, params, prvVal);

		return ret;
	}
}