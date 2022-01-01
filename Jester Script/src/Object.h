#ifndef OBJECT_H
#define OBJECT_H

#include "Types.h"

namespace jts {
	
	// Jester Script object
	struct Obj {
		Type type;
		Spec spec;

		str symbol;

		union {
			// Integrals

			j_char _char;
			j_bool _bool;
			j_int   _int;
			j_float _float;

			// Structures

			str* _string;
			JtsType* _jtsType;
			CppClass* _cppType;

			// Callables

			void (*_native)(VM*, Obj*, ObjNode*, bool);
			CppFn* _cppFn;
			JtsFn* _jtsFn;

			// Lists

			ObjNode* _args;
			Obj* _quote;
		};

		size_t* refCount = 0;
		bool constant = false;
		bool initialized = false;

		void assert(bool cond, str message);
	};

	static Obj* NIL = new Obj{ Type::NIL, Spec::SYMBOL };

	// Jester Script non-destructive object wrapper

	struct ObjNode {
		ObjNode() = default;

		ObjNode(Obj* obj) :
			value(obj) {
		}

		ObjNode* next = nullptr;

		Obj* value = nullptr;
	};
}

#endif

