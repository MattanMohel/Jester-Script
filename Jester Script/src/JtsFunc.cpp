#include "JtsFunc.h"
#include "Execute.h"
#include "Object.h"
#include "Operations.h"

namespace jts
{
	Obj* ExecJtsFunc(ObjNode* func)
	{
		ObjNode* block = func->value->_jtsFunc->codeBlock;	

		if (func->value->_jtsFunc->params)
		{
			ObjNode* params = func->value->_jtsFunc->params;
			ObjNode* paramInput = func->args;

			BinaryOpObj<BinaryOp::SET>(params, paramInput);
			params = params->args;
			
			while (params)
			{
				paramInput = paramInput->next;
				BinaryOpObj<BinaryOp::SET>(params, paramInput);
				params = params->next;
			}
		}

		while (block->next)
		{
			EvalObj(block);

			block = block->next;
		}

		return EvalObj(block);
	}
}