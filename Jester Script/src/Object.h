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

			ObjNode* _quote;
		};

		union // Callables
		{
			Obj* (*_native)(ObjNode* args);
			Func* _jtsFunc;
		};

		ObjNode* cell = nullptr;

		Flag<SFlag, ENUM_SIZE(SFlag)> flag;
		bool initialized = false;
	};

	static Obj* NIL = new Obj { Type::NIL, Spec::SYMBOL, FnType::NIL };

	struct ObjNode
	{
		ObjNode(bool invoc = false, bool quoted = false) : 
			value(new Obj()), invocation(invoc), quoted(quoted)
		{}		
		
		ObjNode(Obj* obj, bool invoc = false, bool quoted = false) :
			value(obj), invocation(invoc), quoted(quoted)
		{}

		ObjNode* next = nullptr;
		ObjNode* args = nullptr;

		bool quoted = false;
		bool invocation = false;

		Obj* value = nullptr;
	};
}

#endif

