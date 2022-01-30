#ifndef JTS_FUNC_H
#define JTS_FUNC_H

#include "Types.h"

namespace jts
{
	struct JtsFn {

		Node* block  = nullptr;	
		Node* params = nullptr;

		str id;
	};	

	struct NatFn {

		Obj* (*_native)(VM*, Node*);

		str id;
	};

	template<typename Fn>
	inline Obj* call(VM* vm, Node* args, Fn* func) {
		static_assert(false, "unsupported function type");
	}

	template<> Obj* call(VM* vm, Node* args, JtsFn* func);
	template<> Obj* call(VM* vm, Node* args, NatFn* func);
}

#endif