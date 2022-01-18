#include "ListOp.h"

#include "core/Object.h"
#include "core/Execute.h"

namespace jts::lst {

	Node* copy(VM* vm, Node* lst, std::function<Obj* (VM*, Obj*)> copy) {
		if (!lst) {
			return nullptr;
		}

		Node* res = nullptr;
		Node** resPtr = &res;

		while (lst) {
			(*resPtr) = env::newNode(vm, copy(vm, lst->val));

			lst = lst->nxt;
			resPtr = &(*resPtr)->nxt;
		}

		return res;
	}

	void free(VM* vm, Node* lst) {
		while (lst) {
			freeObj(vm, lst->val);

			env::releaseNode(vm, lst);
			env::releaseObj(vm, lst->val);

			shift(&lst);
		}
	}

	void forEach(VM* vm, Node* lst, std::function<void(VM*, Obj*)> each) {
		if (!lst) {
			return;
		}

		while (lst) {
			each(vm, lst->val);
			lst = lst->nxt;
		}
	}	
	
	void forEach(VM* vm, Node* lst, std::function<void(VM*, Node*)> each) {
		if (!lst) {
			return;
		}

		while (lst) {
			each(vm, lst);
			lst = lst->nxt;
		}
	}

	Node* eval(VM* vm, Node* lst) {
		return copy(vm, lst, [](VM* vm, Obj* elm) {
			return env::newObj(vm, evalObj(vm, elm));
		});
	}	
	
	Node* evalSelf(VM* vm, Node* lst) {
		forEach(vm, lst, [](VM* vm, Obj* elm) {
			if (!isMutable(elm)) return;

			setObj(vm, elm, evalObj(vm, elm));
		});

		return lst;
	}
}