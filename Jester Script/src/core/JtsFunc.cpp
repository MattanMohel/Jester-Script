#include "JtsFunc.h"
#include "Execute.h"
#include "Object.h"
#include "VM.h"

#include "util/ScopeHelper.h"
#include "util/ObjectOp.h"
#include "util/ListOp.h"

namespace jts {

	template<> Obj* call(VM* vm, Node* args, JtsFn* func) {

		BINDING(bind, func->params, args);

		Node* blockPtr = func->block;

		while (blockPtr->nxt) {
			evalObj(vm, blockPtr->val);
			shift(&blockPtr);
		}

		return evalObj<true>(vm, blockPtr->val);
	}

	template<> Obj* call(VM* vm, Node* args, NatFn* func) {
		return func->_native(vm, args);
	}
}