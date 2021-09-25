#ifndef OBJECT_H
#define OBJECT_H

#include "Types.h"

namespace jts
{
	struct Obj
	{
		Type type;
		Spec spec;

		str symbol;
	 
		union 
		{
			// Values
			char _char;
			bool _bool;
			int _int;
			float _float;

			// Argument holders
			ObjNode* _args;

			// Callables
			Obj* (*_native)(ObjNode* ret, ObjNode* args);
			Func* _jtsFunc;
		};
		
		ObjNode* ret = nullptr;
	};

	static Obj* NIL = new Obj { Type::NIL, Spec::SYMBOL };

	struct ObjNode
	{
		ObjNode() : 
			value(new Obj())
		{}
		
		ObjNode(Obj* obj) :
			value(obj)
		{}
		
		ObjNode(Spec spec) :
			value(new Obj { Type::NIL, spec })
		{}

		ObjNode* next = nullptr;

		Obj* value = nullptr;
	};
}

#endif

