#ifndef STANDARD_H
#define STANDARD_H

#include "../src/VM.h"
#include "../src/Types.h"
#include "../src/Object.h"
#include "../src/Operations.h"
#include "../src/Execute.h"
#include "../src/JtsFunc.h"
#include "../src/Log.h"
#include "../src/JtsMacro.h"

#include <iostream>

using namespace jts;

namespace lib
{
	inline void StandardLib(VM* vm)
	{
		env::AddSymbol(vm, "nil", env::AddConst(nullptr));

		env::AddSymbol(vm, "quote", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			return UnaryOp<Unary::QUOTE>(args->value);
		}));

		env::AddSymbol(vm, "set", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			return BinaryOp<Binary::SET>(EvalObj(args, eval), EvalObj(args->next, eval));
		}));
		
		env::AddSymbol(vm, "first", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			auto* elem = EvalObj(args, eval)->_args;

			if (!elem) return NIL;

			return elem->value;
		}));			
		
		env::AddSymbol(vm, "rest", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			auto* elem = EvalObj(args, eval)->_args;

			if (!elem || !elem->next) return NIL;

			return elem->next->value;
		}));			
		
		env::AddSymbol(vm, "...", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			auto* head = EvalObj(args, eval);
			auto* elem = head->_args;

			if (!elem || !elem->next) return NIL;

			head->_args = elem->next;

			return elem->value;
		}));				
		
		env::AddSymbol(vm, "nth", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			Obj* head = EvalObj(args->next, eval);

			if (!head) return NIL;

			auto* elem = head->_args;
			
			for (size_t i = 0; i < CastObj<j_int>(EvalObj(args, eval)); ++i)
			{
				elem = elem->next;

				if (!elem) return NIL;
			}

			return elem->value;
		}));		
		
		env::AddSymbol(vm, "append", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			// (append value list)

			Obj* head = EvalObj(args->next, eval);

			if (head->type != Type::LIST)
			{
				head->type = Type::LIST;

				head->_args = new ObjNode(env::glbl_objPool.acquire());

				return BinaryOp<Binary::SET>(head->_args->value, EvalObj(args, eval));
			}

			auto* elem = head->_args;
			
			while (elem->next)
			{
				elem = elem->next;
			}

			elem->next = new ObjNode(env::glbl_objPool.acquire());
			
			return BinaryOp<Binary::SET>(elem->next->value, EvalObj(args, eval));
		}));		
		
		env::AddSymbol(vm, "insert", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			// (insert index value list)

			Obj* head = EvalObj(args->next->next, eval);
			 
			if (!head) return NIL;

			auto* elem = head->_args;
			
			for (size_t i = 1; i < CastObj<j_int>(EvalObj(args, eval)); ++i)
			{
				elem = elem->next;

				if (!elem) return NIL;
			}

			auto* prev = elem->next;
			elem->next = new ObjNode(new Obj());
			elem->next->next = prev;

			return BinaryOp<Binary::SET>(elem->next->value, EvalObj(args->next, eval));
		}));

		env::AddSymbol(vm, "defn", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			// (defn id (args) code)
			Obj* func = args->value;

			func->spec = Spec::SYMBOL;
			func->type = Type::JTS_FN;

			func->_jtsFunc = new Func();
			func->_jtsFunc->params = args->next;
			func->_jtsFunc->codeBlock = args->next->next;

			return func;
		}));

		env::AddSymbol(vm, "defm", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			// (defn id (args) code)
			Obj* func = args->value;

			func->spec = Spec::SYMBOL;
			func->type = Type::MACRO;

			func->_jtsMacro = new Macro();
			func->_jtsMacro->params = args->next;

			func->_jtsMacro->codeBlock = args->next->next;

			return func;
		}));					
		
		env::AddSymbol(vm, "eval", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			return EvalObj(args, true);
		}));		
					
		env::AddSymbol(vm, "loop", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			// (loop cond code)

			auto* cond = args;
			auto* block = args->next;

			bool state = isTrue(EvalObj(cond, eval));

			while (state)
			{
				while (block->next)
				{
					EvalObj(block, eval);
					block = block->next;
				}

				state = isTrue(EvalObj(cond, eval));

				Obj* loopRet = EvalObj(block, eval);

				if (!state) return loopRet;

				block = args->next;
			}
		}));	
		
		env::AddSymbol(vm, "iterate", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			// (loop value code)

			auto* list = EvalObj(args->next, eval)->_args;
			auto* block = args->next->next;

			while (list)
			{
				BinaryOp<Binary::SET>(EvalObj(args, eval), EvalObj(list, eval));

				while (block->next)
				{
					EvalObj(block, eval);
					block = block->next;
				}

				Obj* loopRet = EvalObj(block, eval);

				if (!list->next) return loopRet;

				list = list->next;
				block = args->next->next;
			}
		}));		
		
		env::AddSymbol(vm, "progn", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			while (args->next)
			{
				EvalObj(args, eval);
				args = args->next;
			}

			return EvalObj(args, eval);
		}));		
		
		env::AddSymbol(vm, "dictionary", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			// (dictionary (key value) (key value)...)

			ret->type = Type::LIST;
			ret->_args = new ObjNode(new Obj());

			auto* elem = ret->_args;

			while (args)
			{
				elem->value->_args = new ObjNode(new Obj());
				elem->value->_args->next = new ObjNode(new Obj());

				BinaryOp<Binary::SET>(elem->value->_args->value, args->value->_args->value);
				BinaryOp<Binary::SET>(elem->value->_args->next->value, args->value->_args->next->value);

				args = args->next;
				elem = elem->next;
			}

			return ret;
		}));		
		
		env::AddSymbol(vm, "hash", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			BinaryOp<Binary::SET>(ret, EvalObj(args, eval));
			return UnaryOp<Unary::HASH>(ret);
		}));

		env::AddSymbol(vm, "print", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			while (args->next)
			{
				PrintObj(EvalObj(args, eval), false);
				args = args->next;
			}

			return 	PrintObj(EvalObj(args, eval), false);
		}));

		env::AddSymbol(vm, "println", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			while (args->next)
			{
				PrintObj(EvalObj(args, eval), false);
				args = args->next;
			}

			return 	PrintObj(EvalObj(args, eval), true);
		}));
	}
}

#endif