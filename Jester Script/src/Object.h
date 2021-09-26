#ifndef OBJECT_H
#define OBJECT_H

#include "Types.h"

#define RetVal(objNode) objNode->value->ret->value

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
			j_int _int;
			j_float _float;

			// Argument holders
			ObjNode* _args;
			Obj* _quote;
			
			// Callables
			Obj* (*_native)(Obj* ret, ObjNode* args, bool eval);
			Macro* _jtsMacro;
			Func* _jtsFunc;
		};
		
		Obj* ret = nullptr;
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

		ObjNode* next = nullptr;

		Obj* value = nullptr;
	};
}

#endif

