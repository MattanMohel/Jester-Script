#include "JtsFunc.h"
#include "Execute.h"
#include "Object.h"
#include "Operations.h"
#include "VM.h"

namespace jts {

	Obj* JtsFn::call(VM* vm, Node* args) {

		Node* block = codeBlock;
		Node* local = params;

		return env::beginScope(vm, local, [&local , &args, &block](VM* vm) {
			
			// assign passed values to params
			listForEach(vm, local, [&args](VM* vm, Node* node) {
				setObj(vm, node->val, evalObj(vm, args->val));
				args = args->nxt;
			});

			while (block->nxt) {
				evalObj(vm, block->val);
				block = block->nxt;
			}

			return setObj(vm, env::newObj(vm), evalObj(vm, block->val));
		});
	}
}