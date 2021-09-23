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

			ObjNode* _list;

			ObjNode* _quote;

			// Callables
			Obj* (*_native)(ObjNode* fn, ObjNode* args);
			Func* _jtsFunc;
		};

		Flag<SFlag, ENUM_SIZE(SFlag)> flag;
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
		ObjNode* args = nullptr;

		Obj* value = nullptr;
	};

	inline ObjNode* CreateNode(Obj* obj, str symbol)
	{
		obj->symbol = symbol;
		
		return new ObjNode(obj);
	}
}

#endif

