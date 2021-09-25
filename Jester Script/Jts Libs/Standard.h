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

		env::AddSymbol(vm, "set", env::AddNative([](ObjNode* ret, ObjNode* args) -> Obj*
		{
			return BinaryOpObj<BinaryOp::SET>(args, args->next);
		}));		
				
		env::AddSymbol(vm, "bind", env::AddNative([](ObjNode* ret, ObjNode* args) -> Obj*
		{
			Obj* first = EvalObj(args);
			first->_args = new ObjNode();

			BinaryOpObj<BinaryOp::SET>(first->_args, args->next);

			return first;
		}));	
		
		env::AddSymbol(vm, "list", env::AddNative([](ObjNode* ret, ObjNode* args) -> Obj*
		{
			ret->value->type = Type::LIST;
			ret->value->spec = Spec::CALL_BEG;

			ret->value->ret = new ObjNode();
			ret->value->_args = new ObjNode();

			BinaryOpObj<BinaryOp::SET>(ret->value->_args, args);
			ret->value->_args->value->symbol = args->value->symbol;
			ret->value->_args->value->ret = new ObjNode();

			auto* cell = ret->value->_args;

			while (args->next)
			{
				cell->next = new ObjNode();

				cell->next->value->ret = new ObjNode();
				cell->next->value->symbol = args->next->value->symbol;

				auto* curArgs = ret->value->_args;

				BinaryOpObj<BinaryOp::SET>(cell->next, args->next);

				ret->value->_args = curArgs;

				cell = cell->next;
				args = args->next;
			}

			return ret->value;
		}));
		
		env::AddSymbol(vm, "nth", env::AddNative([](ObjNode* ret, ObjNode* args) -> Obj*
		{
			auto* elem = EvalObj(args->next)->_args;

			for (int i = 0; i < CastObj<int>(EvalObj(args)); ++i)
			{
				elem = elem->next;

				if (!elem) return NIL;
			}

			return elem->value;
		}));		
		
		env::AddSymbol(vm, "rest", env::AddNative([](ObjNode* ret, ObjNode* args) -> Obj*
		{
			auto* rest = EvalObj(args)->_args->next;

			if (!rest) return NIL;
			return rest->value;
		}));			
		
		env::AddSymbol(vm, "head", env::AddNative([](ObjNode* ret, ObjNode* args) -> Obj*
		{
			auto* head = args->value->_args;

			if (!head) return NIL;
			return head->value;
		}));		
		
		env::AddSymbol(vm, "eval", env::AddNative([](ObjNode* ret, ObjNode* args) -> Obj*
		{
			RetOf(args) = new Obj();
			return EvalObj(args, true);
		}));

		env::AddSymbol(vm, "defn", env::AddNative([](ObjNode* ret, ObjNode* args) -> Obj*
		{
			Obj* func = EvalObj(args);

			func->spec = Spec::SYMBOL;
			func->type = Type::JTS;

			func->_jtsFunc = new Func();
			func->_jtsFunc->params = args->next;
			func->_jtsFunc->codeBlock = args->next->next;

			return func;
		}));		
		
		env::AddSymbol(vm, "loop", env::AddNative([](ObjNode* ret, ObjNode* args) -> Obj*
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
		
		env::AddSymbol(vm, "progn", env::AddNative([](ObjNode* ret, ObjNode* args) -> Obj*
		{
			while (args->next)
			{
				EvalObj(args);
				args = args->next;
			}

			return EvalObj(args);
		}));

		env::AddSymbol(vm, "print", env::AddNative([](ObjNode* ret, ObjNode* args) -> Obj*
		{
			while (args->next)
			{
				PrintObj(EvalObj(args), false);
				args = args->next;
			}

			return 	PrintObj(EvalObj(args), false);
		}));

		env::AddSymbol(vm, "println", env::AddNative([](ObjNode* ret, ObjNode* args) -> Obj*
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