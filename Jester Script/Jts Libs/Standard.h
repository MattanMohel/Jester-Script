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

		env::AddSymbol(vm, "set", env::AddNative([](ObjNode* fn, ObjNode* args) -> Obj*
		{
			return BinaryOpObj<BinaryOp::SET>(args, args->next);
		}));		
				
		//env::AddSymbol(vm, "bind", env::AddNative([](ObjNode* fn, ObjNode* args) -> Obj*
		//{
		//	auto* beg = EvalObj(in->args);
		//	beg->cell = new ObjNode();

		//	BinaryOpObj<BinaryOp::SET>(beg->cell, in->args->next);

		//	return beg;
		//}));	
		//
		//env::AddSymbol(vm, "list", env::AddNative([](ObjNode* fn, ObjNode* args) -> Obj*
		//{
		//	auto* cell = in->args;
		//	auto* args = in->args;

		//	while (args->next)
		//	{
		//		cell->value->cell = new ObjNode();
		//		BinaryOpObj<BinaryOp::SET>(cell->value->cell, args->next);
		//		cell = cell->value->cell;
		//		args = args->next;
		//	}

		//	return in->args->value;
		//}));
		//
		//env::AddSymbol(vm, "nth", env::AddNative([](ObjNode* fn, ObjNode* args) -> Obj*
		//{
		//	ObjNode* init = in->args->next;

		//	for (int i = 0; i < CastObj<int>(EvalObj(in->args)); ++i)
		//	{
		//		init = init->value->cell;

		//		if (!init) return NIL;
		//	}

		//	return init->value;
		//}));		
		//
		//env::AddSymbol(vm, "rest", env::AddNative([](ObjNode* fn, ObjNode* args) -> Obj*
		//{
		//	auto* rest = EvalObj(in->args);

		//	if (!rest->cell) return NIL;
		//	return rest->cell->value;
		//}));		
		//
		//env::AddSymbol(vm, "eval", env::AddNative([](ObjNode* fn, ObjNode* args) -> Obj*
		//{
		//	if (in->args->value->type == Type::QUOTE && in->args->value->_quote->invocation)
		//	{
		//		return ExecObj(in->args->value->_quote);
		//	}

		//	return in->args->value;
		//}));

		env::AddSymbol(vm, "defn", env::AddNative([](ObjNode* fn, ObjNode* args) -> Obj*
		{
			Obj* func = EvalObj(args);

			func->spec = Spec::SYMBOL;
			func->fnType = FnType::JTS;

			func->_jtsFunc = new Func();
			func->_jtsFunc->params = args->next;
			func->_jtsFunc->codeBlock = args->next->next;

			return func;
		}));		
		
		env::AddSymbol(vm, "loop", env::AddNative([](ObjNode* fn, ObjNode* args) -> Obj*
		{
			auto* cond = args;

			args = args->next;
			auto* blockStart = args;

			bool condState = isTrue(cond);

			while (condState)
			{
				while (args->next)
				{
					EvalObj(args);
					args = args->next;
				}

				Obj* ret = EvalObj(args);

				condState = isTrue(cond);

				if (!condState) return ret;

				args = blockStart;
			}
		}));		
		
		env::AddSymbol(vm, "progn", env::AddNative([](ObjNode* fn, ObjNode* args) -> Obj*
		{
			while (args->next)
			{
				EvalObj(args);
				args = args->next;
			}

			return EvalObj(args);
		}));

		env::AddSymbol(vm, "print", env::AddNative([](ObjNode* fn, ObjNode* args) -> Obj*
		{
			while (args->next)
			{
				PrintObj(EvalObj(args), false);
				args = args->next;
			}

			return 	PrintObj(EvalObj(args), false);
		}));

		env::AddSymbol(vm, "println", env::AddNative([](ObjNode* fn, ObjNode* args) -> Obj*
		{
			while (args->next)
			{
				PrintObj(EvalObj(args), false);
				args = args->next;
			}

			return 	PrintObj(EvalObj(args), true);
		}));
	}
}

#endif