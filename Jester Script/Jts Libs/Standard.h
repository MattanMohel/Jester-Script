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

				// So recursive fn->_list is maintained
				auto* curArgs = fn->value->_list;
				auto* curList =   cell->next;

				curList->value = EvalObj(args->next);
				fn->value->_list = curArgs;

				cell = cell->next;
				args = args->next;
			}

			return fn->value;
		}));
		
		env::AddSymbol(vm, "nth", env::AddNative([](ObjNode* fn, ObjNode* args) -> Obj*
		{
			auto* elem = EvalObj(args->next)->_list;

			for (int i = 0; i < CastObj<int>(EvalObj(args)); ++i)
			{
				elem = elem->next;

				if (!elem) return NIL;
			}

			return elem->value;
		}));		
		
		env::AddSymbol(vm, "rest", env::AddNative([](ObjNode* fn, ObjNode* args) -> Obj*
		{
			auto* rest = EvalObj(args)->_list->next;

			if (!rest) return NIL;
			return rest->value;
		}));			
		
		env::AddSymbol(vm, "head", env::AddNative([](ObjNode* fn, ObjNode* args) -> Obj*
		{
			auto* head = args->value->_list;

			if (!head) return NIL;
			return head->value;
		}));		
		
		env::AddSymbol(vm, "eval", env::AddNative([](ObjNode* fn, ObjNode* args) -> Obj*
		{
			if (args->value->type == Type::LIST) return ExecObj(args);

			return EvalObj(args);
		}));

		env::AddSymbol(vm, "defn", env::AddNative([](ObjNode* fn, ObjNode* args) -> Obj*
		{
			Obj* func = EvalObj(args);

			func->spec = Spec::SYMBOL;
			func->type = Type::JTS;

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