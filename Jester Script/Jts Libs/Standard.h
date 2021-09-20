#ifndef STANDARD_H
#define STANDARD_H

#include "../src/VM.h"
#include "../src/Types.h"
#include "../src/Object.h"
#include "../src/Operations.h"
#include "../src/Execute.h"
#include "../src/JtsFunc.h"
#include "../src/Log.h"

#include <iostream>

using namespace jts;

namespace lib
{
	inline void StandardLib(VM* vm)
	{
		env::AddSymbol(vm, "nil", env::AddConst(nullptr));

		env::AddSymbol(vm, "set", env::AddNative([](ObjNode* in) -> Obj*
		{
			return BinaryOpObj<BinaryOp::SET>(in->args, in->args->next);
		}));		
		
		env::AddSymbol(vm, "setv", env::AddNative([](ObjNode* in) -> Obj*
		{
			return BinaryOpObj<BinaryOp::SET_VAL>(in->args, in->args->next);
		}));	
		
		env::AddSymbol(vm, "setl", env::AddNative([](ObjNode* in) -> Obj*
		{
			return BinaryOpObj<BinaryOp::SET_CON>(in->args, in->args->next);
		}));
				
		env::AddSymbol(vm, "bind", env::AddNative([](ObjNode* in) -> Obj*
		{
			auto* beg = EvalObj(in->args);
			beg->cell = new ObjNode();

			BinaryOpObj<BinaryOp::SET>(beg->cell, in->args->next);

			return beg;
		}));	
		
		env::AddSymbol(vm, "list", env::AddNative([](ObjNode* in) -> Obj*
		{
			auto* cell = in->args;
			auto* args = in->args;

			while (args->next)
			{
				cell->value->cell = new ObjNode();
				BinaryOpObj<BinaryOp::SET>(cell->value->cell, args->next);
				cell = cell->value->cell;
				args = args->next;
			}

			return in->args->value;
		}));
		
		env::AddSymbol(vm, "nth", env::AddNative([](ObjNode* in) -> Obj*
		{
			ObjNode* init = in->args->next;

			for (int i = 0; i < CastObj<int>(EvalObj(in->args)); ++i)
			{
				init = init->value->cell;

				if (!init) return NIL;
			}

			return init->value;
		}));		
		
		env::AddSymbol(vm, "rest", env::AddNative([](ObjNode* in) -> Obj*
		{
			auto* rest = EvalObj(in->args);

			if (!rest->cell) return NIL;
			return rest->cell->value;
		}));		
		
		env::AddSymbol(vm, "eval", env::AddNative([](ObjNode* in) -> Obj*
		{
			if (in->args->value->type == Type::QUOTE && in->args->value->_quote->invocation)
			{
				return ExecObj(in->args->value->_quote);
			}

			return in->args->value;
		}));

		env::AddSymbol(vm, "defn", env::AddNative([](ObjNode* in) -> Obj*
		{
			Obj* fn = in->args->value;

			fn->spec = Spec::SYMBOL; 
			fn->fnType = FnType::JTS;

			fn->_jtsFunc = new Func();
			fn->_jtsFunc->params = in->args->next;
			fn->_jtsFunc->codeBlock = in->args->next->next;

			return fn;
		}));		
		
		env::AddSymbol(vm, "loop", env::AddNative([](ObjNode* in) -> Obj*
		{
			auto* cond = in->args;
			auto* block = cond->next;
			auto* blockStart = block;

			bool condState = isTrue(cond);

			while (condState)
			{
				while (block->next)
				{
					EvalObj(block);
					block = block->next;
				}

				Obj* ret = EvalObj(block);

				condState = isTrue(cond);

				if (!condState) return ret;

				block = blockStart;
			}
		}));		
		
		env::AddSymbol(vm, "progn", env::AddNative([](ObjNode* in) -> Obj*
		{
			auto* beg = in->args;

			while (beg->next)
			{
				EvalObj(beg);
				beg = beg->next;
			}

			return EvalObj(beg);
		}));



		env::AddSymbol(vm, "print", env::AddNative([](ObjNode* in) -> Obj*
		{
			auto* beg = in->args;

			while (beg->next)
			{
				PrintObj(EvalObj(beg), false);
				beg = beg->next;
			}

			return 	PrintObj(EvalObj(beg), false);
		}));

		env::AddSymbol(vm, "println", env::AddNative([](ObjNode* in) -> Obj*
		{
			auto* beg = in->args;

			while (beg->next)
			{
				PrintObj(EvalObj(beg), false);
				beg = beg->next;
			}

			return 	PrintObj(EvalObj(beg), true);
		}));
	}
}

#endif