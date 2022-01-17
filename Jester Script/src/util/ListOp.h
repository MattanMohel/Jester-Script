#pragma once

#include "core/Types.h"
#include "ObjectOp.h"

namespace jts::lst {

	Node* copy(VM* vm, Node* lst, std::function<Obj* (VM*, Obj*)> copy =
		[](VM* vm, Obj* obj) {
		return env::newObj(vm, obj);
	});

	void forEach(VM* vm, Node* lst, std::function<void(VM*, Obj*)> each);
	void forEach(VM* vm, Node* lst, std::function<void(VM*, Node*)> each);
}