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

			char _char;
			bool _bool;
			j_int _int;
			j_float _float;

			// Callable values

			Obj* (*_native)(Obj*, ObjNode*, bool);
			CppFn* _cppFunc;
			Macro* _jtsMacro;
			Func* _jtsFunc;

			// List values

			ObjNode* _args;
			Obj* _quote;			
		};
		
		// callable return value
		Obj* ret = nullptr;
	};

	static Obj* NIL = new Obj { Type::NIL, Spec::SYMBOL };

	// Jester Script non-destructive object wrapper

	struct ObjNode
	{
		ObjNode() : 
			value(new Obj())
		{}
		
		ObjNode(Obj* obj) :
			value(obj)
		{}

		ObjNode* next = nullptr;

		Obj* value = nullptr;
	};
}

#endif

