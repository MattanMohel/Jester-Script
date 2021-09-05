#ifndef OBJECT_H
#define OBJECT_H

#include "Types.h"

namespace jts
{
	struct Obj
	{
		Obj* head;

		Type type;

		void (*func)(Obj*);

		union
		{
			char* _char;
			bool* _bool;
			int* _int;
			float* _float;
		};
	};

}

#endif

