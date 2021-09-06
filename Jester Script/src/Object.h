#ifndef OBJECT_H
#define OBJECT_H

#include "Types.h"

namespace jts
{
	struct Obj
	{
		Obj* rest;

		Type type;
		Spec spec;

		bool argsEnd = false;

		union // Values
		{
			char _char;
			bool _bool;
			int _int;
			float _float;
		};

		union // Callables
		{
			void (*_native)(Obj*);
		};
	};
}

#endif

