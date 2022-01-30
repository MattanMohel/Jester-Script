#include "JtsFunc.h"
#include "Execute.h"
#include "Object.h"
#include "VM.h"

#include "util/ObjectOp.h"
#include "util/ListOp.h"

namespace jts {

	template<> Obj* call(VM* vm, Node* args, JtsFn* func) {

		Node* prvVal = env::bindEnv(vm, func->params, lst::eval(vm, args));

		Node* blockPtr = func->block;

		while (blockPtr->nxt) {
			evalObj(vm, blockPtr->val);
			shift(&blockPtr);
		}

		Obj* ret = evalObj<true>(vm, blockPtr->val);

		env::unbindEnv(vm, func->params, prvVal);

		return ret;
	}

	template<> Obj* call(VM* vm, Node* args, NatFn* func) {
		return func->_native(vm, args);
	}
}