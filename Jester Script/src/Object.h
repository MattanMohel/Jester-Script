#ifndef OBJECT_H
#define OBJECT_H

#include "Types.h"

namespace jts
{
	struct Obj
	{
		Type type;
		Spec spec;
		FnType fnType;

		union // Values
		{
			char _char;
			bool _bool;
			int _int;
			float _float;
		};

		union // Callables
		{
			Obj* (*_native)(ObjNode*);
		};
	};

	struct ObjNode
	{
		ObjNode() : value(new Obj())
		{}		
		
		ObjNode(Obj* obj) : value(obj)
		{}

		ObjNode* next = nullptr;
		ObjNode* args = nullptr;

		Obj* value = nullptr;

	};
}

#endif

