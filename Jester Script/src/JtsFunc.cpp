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
			auto* paramsIn = args->next;
			auto* paramCur = params->value->_args;

			while (paramCur)
			{
				BinaryOp<Binary::SET>(paramCur->value, EvalObj(paramsIn, eval));

				paramCur = paramCur->next;

				paramsIn = paramsIn->next;
			}
		}

		auto* block = codeBlock;

		while (block->next)
		{
			EvalObj(block, eval);
			block = block->next;
		}

		return 	EvalObj(block, eval);
	}	
}