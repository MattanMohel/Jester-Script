#ifndef OBJECT_H
#define OBJECT_H

#include "Types.h"

namespace jts
{
	struct Obj
	{
		// Object's arguments
		Obj* args;
		// Next object in scope
		Obj* next;

		Type type;
		Spec spec;

		union // Values
		{
			char _char;
			bool _bool;
			int _int;
			float _float;
		};

		union // Callables
		{
			Obj* (*_native)(Obj*);
		};
	};
}

#endif

