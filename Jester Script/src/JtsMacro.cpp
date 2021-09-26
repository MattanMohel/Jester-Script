#include "JtsMacro.h"
#include "Execute.h"
#include "Object.h"
#include "Operations.h"

namespace jts
{
	Obj* Macro::operator() (ObjNode* args, bool eval)
	{
		// (defm id (args) code)

		if (params)
		{
			auto* paramsIn = args->next;
			auto* paramBeg = params->value->_args;

			while (paramBeg)
			{
				BinaryOpObj<BinaryOp::SET>(paramBeg->value, Quote(paramsIn->value->ret, paramsIn, false));

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

	Obj* Quote(Obj* ret, ObjNode* args, bool eval)
	{
		ObjNode* cell = nullptr;

		switch (args->value->spec)
		{
			case Spec::HEAD:

				args = args->value->_args;

				ret->type = Type::LIST;
				ret->spec = Spec::HEAD;

				ret->ret = new Obj();

				ret->_args = new ObjNode(new Obj { Type::QUOTE, Spec::VALUE });
				ret->_args->value->_quote = args->value;

				cell = ret->_args;

				while (args->next)
				{
					cell->next = new ObjNode(Quote(args->next->value->ret, args->next, eval));

					args = args->next;

					cell = cell->next;
				}

				return ret;

			default:

				Obj* quote = new Obj { Type::QUOTE, Spec::VALUE };
				quote->_quote = args->value;

				return quote;
		}
	}
}