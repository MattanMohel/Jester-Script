#include "JtsMacro.h"
#include "Execute.h"
#include "Object.h"
#include "Operations.h"

namespace jts
{
	Obj* Macro::Call (ObjNode* args, bool eval)
	{
		// (defm id (args) code)

		if (params)
		{
			auto* paramsIn = args->next;
			auto* paramBeg = params->value->_args;

			while (paramBeg)
			{
				BinaryOp<Binary::SET>(paramBeg->value, BinaryOp<Binary::QUOTE>(paramsIn->value->ret, paramsIn->value));

				paramBeg = paramBeg->next;

				paramsIn = paramsIn->next;
			}
		}

		auto* block = codeBlock;

		while (block->next)
		{
			EvalObj(block, true);
			block = block->next;
		}

		return 	EvalObj(block, true);
	}
}