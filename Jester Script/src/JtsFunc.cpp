#include "JtsFunc.h"
#include "Execute.h"
#include "Object.h"

namespace jts
{
	Obj* ExecJtsFunc(ObjNode* func)
	{
		ObjNode* block = func->value->_jtsFunc->codeBlock;

		while (block->next)
		{
			ExecObj(block);

			block = block->next;
		}

		return ExecObj(block);
	}
}