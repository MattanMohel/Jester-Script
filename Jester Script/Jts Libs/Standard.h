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

		// (quote target)
		env::AddSymbol(vm, "quote", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			return UnaryOp<Unary::QUOTE>(args->value);
		}));

		// (set to-set setter)
		env::AddSymbol(vm, "set", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			return BinaryOp<Binary::SET>(EvalObj(args->value, eval), EvalObj(args->next->value, eval));
		}));
		
		// (head list)
		env::AddSymbol(vm, "head", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			auto* elem = EvalObj(args->value, eval)->_args;

			if (!elem) return NIL;

			return elem->value;
		}));			
		
		// (rest list)
		env::AddSymbol(vm, "rest", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			auto* elem = EvalObj(args->value, eval)->_args;

			if (!elem || !elem->next) return NIL;

			return elem->next->value;
		}));			
		
		// (nth index list)
		env::AddSymbol(vm, "nth", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			Obj* head = EvalObj(args->next->value, eval);

			if (!head) return NIL;

			auto* elem = head->_args;
			
			for (size_t i = 0; i < CastObj<j_int>(EvalObj(args->value, eval)); ++i)
			{
				elem = elem->next;

				if (!elem) return NIL;
			}

			return elem->value;
		}));		
		
		// (append index list)
		env::AddSymbol(vm, "append", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			Obj* head = EvalObj(args->next->value, eval);

			// If object isn't a list, convert it and append value

			if (head->type != Type::LIST)
			{
				head->type = Type::LIST;

				head->_args = env::AcquireNode();

				return BinaryOp<Binary::SET>(head->_args->value, EvalObj(args->value, eval));
			}

			auto* elem = head->_args;
			
			while (elem->next)
			{
				elem = elem->next;
			}

			elem->next = env::AcquireNode();
			
			return BinaryOp<Binary::SET>(elem->next->value, EvalObj(args->value, eval));
		}));		
		
		// (insert index value list)
		env::AddSymbol(vm, "insert", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			Obj* head = EvalObj(args->next->next->value, eval);
			 
			if (!head) return NIL;

			auto* elem = head->_args;
			
			for (size_t i = 1; i < CastObj<j_int>(EvalObj(args->value, eval)); ++i)
			{
				elem = elem->next;

				if (!elem) return NIL;
			}

			auto* prev = elem->next;
			elem->next = env::AcquireNode();
			elem->next->next = prev;

			return BinaryOp<Binary::SET>(elem->next->value, EvalObj(args->next->value, eval));
		}));

		// (function id (params) body)
		env::AddSymbol(vm, "function", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			Obj* func = args->value;

			func->spec = Spec::SYMBOL;
			func->type = Type::JTS_FN;

			func->_jtsFunc = new Func();
			func->_jtsFunc->params = args->next;
			func->_jtsFunc->codeBlock = args->next->next;

			return func;
		}));

		// (macro id (params) body)
		env::AddSymbol(vm, "macro", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			Obj* func = args->value;

			func->spec = Spec::SYMBOL;
			func->type = Type::MAC_FN;

			func->_jtsMacro = new Macro();
			func->_jtsMacro->params = args->next;
			func->_jtsMacro->codeBlock = args->next->next;

			// set arguments to type quote

			args = args->next->value->_args;

			while (args)
			{
				args->value->type = Type::QUOTE;
				args = args->next;
			}

			return func;
		}));					
		
		env::AddSymbol(vm, "eval", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			return EvalObj(args->value, true);
		}));		
					
		env::AddSymbol(vm, "loop", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			// (loop cond code)

			auto* cond = args;
			auto* block = args->next;

			bool state = isTrue(EvalObj(cond->value, eval));

			while (state)
			{
				while (block->next)
				{
					EvalObj(block->value, eval);
					block = block->next;
				}

				state = isTrue(EvalObj(cond->value, eval));

				Obj* loopRet = EvalObj(block->value, eval);

				if (!state) return loopRet;

				block = args->next;
			}
		}));	
		
		env::AddSymbol(vm, "iterate", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			// (loop value code)

			auto* list = EvalObj(args->next->value, eval)->_args;
			auto* block = args->next->next;

			while (list)
			{
				BinaryOp<Binary::SET>(EvalObj(args->value, eval), EvalObj(list->value, eval));

				while (block->next)
				{
					EvalObj(block->value, eval);
					block = block->next;
				}

				Obj* loopRet = EvalObj(block->value, eval);

				if (!list->next) return loopRet;

				list = list->next;
				block = args->next->next;
			}
		}));		
		
		env::AddSymbol(vm, "progn", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			while (args->next)
			{
				EvalObj(args->value, eval);
				args = args->next;
			}

			return EvalObj(args->value, eval);
		}));		
			
		env::AddSymbol(vm, "print", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			while (args->next)
			{
				PrintObj(EvalObj(args->value, eval), false);
				args = args->next;
			}

			return 	PrintObj(EvalObj(args->value, eval), false);
		}));

		env::AddSymbol(vm, "println", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			while (args->next)
			{
				PrintObj(EvalObj(args->value, eval), false);
				args = args->next;
			}

			return 	PrintObj(EvalObj(args->value, eval), true);
		}));
	}
}

#endif