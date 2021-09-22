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
				
		env::AddSymbol(vm, "bind", env::AddNative([](ObjNode* fn, ObjNode* args) -> Obj*
		{
			Obj* first = EvalObj(args);
			first->_list = new ObjNode();

			BinaryOpObj<BinaryOp::SET>(first->_list, args->next);

			return first;
		}));	
		
		env::AddSymbol(vm, "list", env::AddNative([](ObjNode* fn, ObjNode* args) -> Obj*
		{
			fn->value->_list = new ObjNode();
			BinaryOpObj<BinaryOp::SET>(fn->value->_list, args);
			fn->value->type = Type::LIST;

			auto* cell = fn->value->_list;

			while (args->next)
			{
				cell->next = new ObjNode();
				BinaryOpObj<BinaryOp::SET>(cell->next, args->next);

				cell = cell->next;
				args = args->next;
			}

			return fn->value;
		}));
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
		env::AddSymbol(vm, "eval", env::AddNative([](ObjNode* fn, ObjNode* args) -> Obj*
		{
			if (args->value->type == Type::LIST)
			{
				switch (args->value->fnType)
				{
					case FnType::NATIVE:

						return args->value->_native(args, args->next);

					case FnType::JTS:

						return ExecJtsFunc(args);

					default: // case C_BRIDGE

						return nullptr;
				}
			}

			if (args->invocation && args->value->fnType != FnType::NIL)
			{
				return ExecObj(args);
			}

			return args->value;
		}));

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