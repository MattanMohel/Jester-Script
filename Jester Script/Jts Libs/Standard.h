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

	inline void StandardLib(VM* vm)
	{
		env::AddSymbol(vm, "quote", env::AddNative(Quote));

		env::AddSymbol(vm, "nil", env::AddConst(nullptr));

		env::AddSymbol(vm, "set", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			return BinaryOpObj<BinaryOp::SET>(EvalObj(args, eval), EvalObj(args->next, eval));
		}));	

		env::AddSymbol(vm, "defn", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			Obj* func = args->value->ret;

			func->spec = Spec::SYMBOL;
			func->type = Type::JTS_FN;

			func->_jtsFunc = new Func();
			func->_jtsFunc->params = args->next;
			func->_jtsFunc->codeBlock = args->next->next;

			return func;
		}));				
		
		env::AddSymbol(vm, "eval", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			return EvalObj(args, true);
		}));		
					
		env::AddSymbol(vm, "loop", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			return nullptr;
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