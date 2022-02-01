#pragma once

#include "core/Types.h"
#include "ObjectOp.h"

namespace jts::lst {

	Node* copy(VM* vm, Node* lst, std::function<Obj* (VM*, Obj*)> copy =
		[](VM* vm, Obj* obj) {
		return env::newObj(vm, obj);
	});

	void free(VM* vm, Node* lst);

	void for_each(VM* vm, Node* lst, std::function<void(VM*, Obj*)> each);
	void for_each(VM* vm, Node* lst, std::function<void(VM*, Node*)> each);

	Node* eval(VM* vm, Node* lst);
	Node* evalSelf(VM* vm, Node* lst);
}