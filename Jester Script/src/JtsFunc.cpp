#include "JtsFunc.h"
#include "Execute.h"
#include "Object.h"
#include "Operations.h"

namespace jts
{
	Obj* Func::operator() (ObjNode* args, bool eval)
	{
		// (defn id (args) code)

		if (params)
		{
			auto* paramsIn = args->next;
			auto* paramBeg = params->value->_args;

			while (paramBeg)
			{
				BinaryOpObj<BinaryOp::SET>(paramBeg->value, EvalObj(paramsIn, eval));

				paramBeg = paramBeg->next;

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