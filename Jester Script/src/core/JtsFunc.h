#ifndef JTS_FUNC_H
#define JTS_FUNC_H

#include "Types.h"

namespace jts
{
	struct JtsFn
	{
		Node* block  = nullptr;	
		Node* params = nullptr;

		Obj* call (VM* vm, Node* args);
	};	
}

#endif