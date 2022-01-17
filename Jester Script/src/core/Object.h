#ifndef OBJECT_H
#define OBJECT_H

#include "Types.h"

namespace jts {
	
	// Jester Script object
	struct Obj {
		Type type = Type::NIL;
		Spec spec = Spec::NIL;

		str symbol = "";

		union {
			// Integrals

			jtsc  _char = 0;
			jtsb  _bool;
			jtsi   _int;
			jtsf _float;

			// Structures

			str* _string;
			JtsType* _jtsType;
			CppClass* _cppType;

			// Callables

			Obj* (*_native)(VM*, Node*);
			CppFn* _cppFn;
			JtsFn* _jtsFn;

			// Lists

			Node* _args;
			Obj* _quote;
		};

		bool constant    = false;
		bool initialized = false;
		size_t* ref    = nullptr;

		void assert(bool cond, str message);
	};

	static Obj* NIL = new Obj{ Type::NIL, Spec::SYMBOL };

	// Jester Script non-destructive object wrapper

	struct Node {
		Node() = default;

		Node(Obj* obj) :
			val(obj) {
		}

		Node* nxt = nullptr;
		Obj* val  = nullptr;
	};

	void  shift (Node** node);
	Node* shiftr(Node** node);
}

#endif

