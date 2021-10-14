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
#include "../src/StrCon.h"

#include <iostream>

using namespace jts;

namespace lib
{
	inline void StandardLib(VM* vm)
	{
		env::addSymbol(vm, "nil", env::addConst(nullptr));

		// (quote target)
		env::addSymbol(vm, "quote", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			return quoteObj(args, eval);
		}));

		// (set to-set setter)
		env::addSymbol(vm, "set", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			return binaryOp<Binary::SET>(evalObj(args->value, eval), evalObj(args->next->value, eval));
		}));
		
		// (head list)
		env::addSymbol(vm, "head", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			auto* elem = evalObj(args->value, eval)->_args;

			if (!elem) return NIL;

			return elem->value;
		}));			
		
		// (rest list)
		env::addSymbol(vm, "rest", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			auto* elem = evalObj(args->value, eval)->_args;

			if (!elem || !elem->next) return NIL;

			return elem->next->value;
		}));			
		
		// (nth index list)
		env::addSymbol(vm, "nth", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			Obj* head = evalObj(args->next->value, eval);

			if (!head) return NIL;

			auto* elem = head->_args;
			
			for (size_t i = 0; i < castObj<j_int>(evalObj(args->value, eval)); ++i)
			{
				elem = elem->next;

				if (!elem) return NIL;
			}

			return elem->value;
		}));		
		
		// (append index list)
		env::addSymbol(vm, "append", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			Obj* head = evalObj(args->next->value, eval);

			// If object isn't a list, convert it and append value

			if (head->type != Type::LIST)
			{
				head->type = Type::LIST;

				head->_args = env::acquireNode();

				return binaryOp<Binary::SET>(head->_args->value, evalObj(args->value, eval));
			}

			auto* elem = head->_args;
			
			while (elem->next)
			{
				elem = elem->next;
			}

			elem->next = env::acquireNode();
			
			return binaryOp<Binary::SET>(elem->next->value, evalObj(args->value, eval));
		}));		
		
		// (insert index value list)
		env::addSymbol(vm, "insert", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			Obj* head = evalObj(args->next->next->value, eval);
			 
			if (!head) return NIL;

			auto* elem = head->_args;
			
			for (size_t i = 1; i < castObj<j_int>(evalObj(args->value, eval)); ++i)
			{
				elem = elem->next;

				if (!elem) return NIL;
			}

			auto* prev = elem->next;
			elem->next = env::acquireNode();
			elem->next->next = prev;

			return binaryOp<Binary::SET>(elem->next->value, evalObj(args->next->value, eval));
		}));

		// (fn id (params) body)
		env::addSymbol(vm, "fn", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
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
		env::addSymbol(vm, "macro", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
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
		
		// (eval target)
		env::addSymbol(vm, "eval", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			return evalObj(evalObj(args->value, false), true);
		}));		
		
		// (loop cond body)
		env::addSymbol(vm, "loop", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			auto* cond = args;
			auto* block = args->next;

			bool state = isTrue(evalObj(cond->value, eval));

			while (state)
			{
				while (block->next)
				{
					evalObj(block->value, eval);
					block = block->next;
				}

				Obj* loopRet = evalObj(block->value, eval);

				state = isTrue(evalObj(cond->value, eval));

				if (!state) return loopRet;

				block = args->next;
			}
		}));	
		
		// (iterate variable list body)
		env::addSymbol(vm, "iterate", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			auto* list = evalObj(args->next->value, eval)->_args;
			auto* block = args->next->next;

			while (list)
			{
				binaryOp<Binary::SET>(evalObj(args->value, eval), evalObj(list->value, eval));

				while (block->next)
				{
					evalObj(block->value, eval);
					block = block->next;
				}

				Obj* loopRet = evalObj(block->value, eval);

				if (!list->next) return loopRet;

				list = list->next;
				block = args->next->next;
			}
		}));		
		
		// (list body...)
		env::addSymbol(vm, "list", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			while (args->next)
			{
				evalObj(args->value, eval);
				args = args->next;
			}

			return evalObj(args->value, eval);
		}));		
		
		// (string-int value)
		env::addSymbol(vm, "string-int", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			return setTo(ret, std::stoi(*evalObj(args->value, eval)->_string));
		}));			
		
		// (string-bool value)
		env::addSymbol(vm, "string-bool", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			return setTo(ret, (bool)std::stoi(*evalObj(args->value, eval)->_string));
		}));			
		
		// (string-char value)
		env::addSymbol(vm, "string-char", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			return setTo(ret, (*evalObj(args->value, eval)->_string)[0]);
		}));		
		
		// (string-int value)
		env::addSymbol(vm, "string-float", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			return setTo(ret, std::stof(*evalObj(args->value, eval)->_string));
		}));			

		// (int value)
		env::addSymbol(vm, "int", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			return setTo(ret, castObj<j_int>(evalObj(args->value, eval)));
		}));			
		
		// (float value)
		env::addSymbol(vm, "float", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			return setTo(ret, castObj<j_float>(evalObj(args->value, eval)));
		}));			
		
		// (char value)
		env::addSymbol(vm, "char", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			return setTo(ret, castObj<j_char>(evalObj(args->value, eval)));
		}));			
		
		// (bool value)
		env::addSymbol(vm, "bool", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			return setTo(ret, castObj<j_bool>(evalObj(args->value, eval)));
		}));			
		
		// (string value)
		env::addSymbol(vm, "string", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			return setTo(ret, new str(toString(evalObj(args->value, eval))));
		}));			
		
		// (find key dict)
		env::addSymbol(vm, "key", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			auto* elem = args->next->value->_args;

			while (elem)
			{
				if (isEqual(evalObj(args->value, eval), elem->value->_args->value))
				{
					return elem->value->_args->next->value;
				}

				elem = elem->next;
			}

			return NIL;
		}));		
		
		// (print args)
		env::addSymbol(vm, "print", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			while (args->next)
			{
				printObj(evalObj(args->value, eval), false);
				args = args->next;
			}

			return 	printObj(evalObj(args->value, eval), false);
		}));

		// (println args)
		env::addSymbol(vm, "println", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			while (args->next)
			{
				printObj(evalObj(args->value, eval), false);
				args = args->next;
			}

			return 	printObj(evalObj(args->value, eval), true);
		}));		
		
		// (input) --> (set in (input))
		env::addSymbol(vm, "input", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			str* input = new str();

			std::getline(std::cin, *input);

			return setTo(ret, input); 
		}));
	}
}

#endif