#include "JtsFunc.h"
#include "Execute.h"
#include "Object.h"
#include "Operations.h"

namespace jts
{
	Obj* Func::Call (ObjNode* args, bool eval)
	{
		// (defn id (args) code)

		if (params)
		{
			auto* paramVal = args->next;
			auto* paramPtr = params->value->_args;

			while (paramPtr)
			{
				BinaryOp<Binary::SET>(paramPtr->value, EvalObj(paramVal, eval));

				paramPtr = paramPtr->next;

				paramVal = paramVal->next;
			}
		}

		auto* block = codeBlock;

		while (block->next)
		{
			EvalObj(block, eval);
			block = block->next;
		}

		return EvalObj(block, eval);
	}
}