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
			EvalObj(block);

			block = block->next;
		}

		return EvalObj(block);
	}
}