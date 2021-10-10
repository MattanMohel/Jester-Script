#include "JtsMacro.h"
#include "Execute.h"
#include "Object.h"
#include "Operations.h"
#include "VM.h"

namespace jts
{
	Obj* Macro::Call(ObjNode* args, bool eval)
	{
		auto* paramPtr = params->value->_args;
		args = args->next;

		if (params->value->_args)
		{
			// assign copy to passed values

			while (args)
			{
				paramPtr->value->_quote = args->value;

				args = args->next;
				paramPtr = paramPtr->next;
			}
		}

		// Execute function body

		auto* block = codeBlock;

		while (block->next)
		{
			EvalObj(block->value, true);
			block = block->next;
		}

		// Evaluate return

		return EvalObj(block->value, true);
	}
}