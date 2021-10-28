#include "JtsMacro.h"
#include "Execute.h"
#include "Object.h"
#include "Operations.h"
#include "VM.h"

namespace jts
{
	Obj* MacFn::call(ObjNode* args, bool eval)
	{
		auto* paramPtr = params->value->_args;

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
			evalObj(block->value, true);
			block = block->next;
		}

		// Evaluate return

		return evalObj(block->value, true);
	}
}