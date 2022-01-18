#include "Object.h"
#include "Log.h"
#include "VM.h"

namespace jts {
	
	void Obj::assert(bool cond, str message) {
		env::assert(cond, mes(message, this->symbol));
	}

	void shift(Node** node) {
		*node = (*node)->nxt;
	}

	Node* shiftr(Node** node) {
		Node* prv = *node;
		*node = (*node)->nxt;
		return prv;
	}

	bool isMutable(Obj* obj) {
		return obj->spec == Spec::SYMBOL && !obj->constant;
	}
}