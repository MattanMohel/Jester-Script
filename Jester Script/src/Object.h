#ifndef OBJECT_H
#define OBJECT_H

#include "Types.h"

namespace jts
{
	// Jester Script object

	struct Obj
	{
		Type type;
		Spec spec;

		str symbol;
	 
		union 
		{
			// Integral values

			j_char _char;
			j_bool _bool;
			j_int   _int;
			j_float _float;
			str* _string;

			// Callable values

			Obj* (*_native)(Obj*, ObjNode*, bool);
			CppFn* _cppFn;
			MacFn* _macFn;
			JtsFn* _jtsFn;

			// List values

			ObjNode* _args;
			Obj* _quote;
		};

		size_t refCount = 0;
	};

	static Obj* NIL = new Obj { Type::NIL, Spec::SYMBOL };

	// Jester Script non-destructive object wrapper

	struct ObjNode
	{
		ObjNode() = default;

		ObjNode(Obj* obj) :
			value(obj)
		{}

		ObjNode* next = nullptr;

		Obj* value = nullptr;
	};
}

#endif

