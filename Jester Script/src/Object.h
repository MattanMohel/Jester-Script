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

		str symbol;

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
		ObjNode() : 
			value(new Obj())
		{}		
		
		ObjNode(Obj* obj, bool invoc = false, bool quoted = false) :
			value(obj), invocation(invoc)
		{}

		ObjNode* next = nullptr;
		ObjNode* args = nullptr;

		bool invocation = false;

		Obj* value = nullptr;
	};

	inline ObjNode* CreateNode(Obj* obj, str symbol, bool invoc, bool quoted)
	{
		obj->symbol = symbol;

		if (quoted)
		{
			Obj* value = new Obj { Type::QUOTE, Spec::LTRL };
			value->_quote = new ObjNode(obj, invoc, quoted);

			return new ObjNode(value);
		}
		
		return new ObjNode(obj, invoc, quoted);
	}
}

#endif

