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

		str* symbol;

		union // Values
		{
			char _char;
			bool _bool;
			int _int;
			float _float;
		};

		union // Callables
		{
			Obj* (*_native)(ObjNode* args);
			Func* _jtsFunc;
		};

		ObjNode* cons = nullptr;

		Flag<SFlag, ENUM_SIZE(SFlag)> flag;
		bool initialized = false;
	};

	static Obj* NIL = new Obj { Type::NIL, Spec::VALUE, FnType::NIL };

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

