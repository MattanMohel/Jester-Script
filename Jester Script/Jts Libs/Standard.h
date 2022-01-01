#ifndef STANDARD_H
#define STANDARD_H

#include "../src/VM.h"
#include "../src/Types.h"
#include "../src/Object.h"
#include "../src/Execute.h"
#include "../src/Operations.h"
#include "../src/JtsFunc.h"
#include "../src/JtsType.h"
#include "../src/CppClass.h"
#include "../src/Log.h"

#include <iostream>

using namespace jts;

namespace lib {
	inline void StandardLib(VM* vm) {

		env::addSymbol(vm, "nil", env::addConst(nullptr));

		// (quote target)
		env::addSymbol(vm, "quote", env::addNative([](Obj* ret, ObjNode* args, bool eval)
		{
			quoteObj(args->value, ret, eval);
		}));
		
		// (set to-set value)
		env::addSymbol(vm, "set", env::addNative([](Obj* ret, ObjNode* args, bool eval) 
		{
			binaryOp<Binary::SET>(ret, binaryOp<Binary::SET>(args->value, evalObj(args->next->value, eval)));
		}));

		// (const to-set value)
		env::addSymbol(vm, "const", env::addNative([](Obj* ret, ObjNode* args, bool eval) 
		{
		#if DEBUG
			args->value->assert(args->value->initialized, "trying to make an initialized value % const");
		#endif

			args->value->constant = true;
			binaryOp<Binary::SET>(ret, binaryOp<Binary::SET>(args->value, evalObj(args->next->value, eval)));
		}));

		// (fn (params) body)
		env::addSymbol(vm, "fn", env::addNative([](Obj* ret, ObjNode* args, bool eval) 
		{
			ret->spec = Spec::SYMBOL;
			ret->type = Type::JTS_FN;

			ret->_jtsFn = new JtsFn();
			ret->_jtsFn->codeBlock = args->next;

			auto param = args->value->_args;

			while (param) {
				param->value = evalObj(param->value);
				param = param->next;
			}

			ret->_jtsFn->params = args;
		}));			
		
		// (defn name (params) body)
		env::addSymbol(vm, "defn", env::addNative([](Obj* ret, ObjNode* args, bool eval) 
		{
			args->value->spec = Spec::SYMBOL;
			args->value->type = Type::JTS_FN;

			args->value->_jtsFn = new JtsFn();
			args->value->_jtsFn->codeBlock = args->next->next;

			auto param = args->next->value->_args;

			while (param) {
				param->value = evalObj(param->value);
				param = param->next;
			}

			args->value->_jtsFn->params = args->next;

			binaryOp<Binary::SET>(ret, args->value);
		}));		

		// (eval target)
		env::addSymbol(vm, "eval", env::addNative([](Obj* ret, ObjNode* args, bool eval) 
		{
			binaryOp<Binary::SET>(ret, evalObj(evalObj(args->value, false), true));
		}));

		// (loop cond body)
		env::addSymbol(vm, "loop", env::addNative([](Obj* ret, ObjNode* args, bool eval) 
		{
			auto cond = args;
			auto block = args->next;

			bool state = isTrue(evalObj(cond->value, eval));

			while (state) {
				while (block->next) {
					evalObj(block->value, eval);
					block = block->next;
				}

				auto loopRet = evalObj(block->value, eval);

				state = isTrue(evalObj(cond->value, eval));

				if (!state) {
					binaryOp<Binary::SET>(ret, loopRet);
					break;
				}

				block = args->next;
			}
		}));

		// (do body...)
		env::addSymbol(vm, "do", env::addNative([](Obj* ret, ObjNode* args, bool eval) 
		{
			while (args->next) {
				evalObj(args->value, eval);
				args = args->next;
			}

			binaryOp<Binary::SET>(ret, evalObj(args->value, eval));
		}));

		// (string-int value)
		env::addSymbol(vm, "string-int", env::addNative([](Obj* ret, ObjNode* args, bool eval) 
		{
			binaryOp<Binary::SET>(ret, setTo(ret, std::stoi(*evalObj(args->value, eval)->_string)));
		}));

		// (string-bool value)
		env::addSymbol(vm, "string-bool", env::addNative([](Obj* ret, ObjNode* args, bool eval) 
		{
			binaryOp<Binary::SET>(ret, setTo(ret, (bool)std::stoi(*evalObj(args->value, eval)->_string)));
		}));

		// (string-char value)
		env::addSymbol(vm, "string-char", env::addNative([](Obj* ret, ObjNode* args, bool eval) 
		{
			binaryOp<Binary::SET>(ret, setTo(ret, (*evalObj(args->value, eval)->_string)[0]));
		}));

		// (string-int value)
		env::addSymbol(vm, "string-float", env::addNative([](Obj* ret, ObjNode* args, bool eval) 
		{
			binaryOp<Binary::SET>(ret, setTo(ret, std::stof(*evalObj(args->value, eval)->_string)));
		}));

		// (int value)
		env::addSymbol(vm, "int", env::addNative([](Obj* ret, ObjNode* args, bool eval) 
		{
			binaryOp<Binary::SET>(ret, setTo(ret, castObj<j_int>(evalObj(args->value, eval))));
		}));

		// (float value)
		env::addSymbol(vm, "float", env::addNative([](Obj* ret, ObjNode* args, bool eval) 
		{
			binaryOp<Binary::SET>(ret, setTo(ret, castObj<j_float>(evalObj(args->value, eval))));
		}));

		// (char value)
		env::addSymbol(vm, "char", env::addNative([](Obj* ret, ObjNode* args, bool eval) 
		{
			binaryOp<Binary::SET>(ret, setTo(ret, castObj<j_char>(evalObj(args->value, eval))));
		}));

		// (bool value)
		env::addSymbol(vm, "bool", env::addNative([](Obj* ret, ObjNode* args, bool eval) 
		{
			binaryOp<Binary::SET>(ret, setTo(ret, castObj<j_bool>(evalObj(args->value, eval))));
		}));

		// (string value)
		env::addSymbol(vm, "string", env::addNative([](Obj* ret, ObjNode* args, bool eval) 
		{
			binaryOp<Binary::SET>(ret, setTo(ret, new str(toString(evalObj(args->value, eval)))));
		}));

		// (print args)
		env::addSymbol(vm, "print", env::addNative([](Obj* ret, ObjNode* args, bool eval) 
		{
			while (args->next) {
				printObj(evalObj(args->value, eval), false);
				args = args->next;
			}

			binaryOp<Binary::SET>(ret, printObj(evalObj(args->value, eval), false));
		}));

		// (println args)
		env::addSymbol(vm, "println", env::addNative([](Obj* ret, ObjNode* args, bool eval) 
		{
			while (args->next) {
				printObj(evalObj(args->value, eval), false);
				args = args->next;
			}

			binaryOp<Binary::SET>(ret, printObj(evalObj(args->value, eval), true));
		}));

		// (input) --> (set in (input))
		env::addSymbol(vm, "input", env::addNative([](Obj* ret, ObjNode* args, bool eval) 
		{
			str* input = new str();

			std::getline(std::cin, *input);

			binaryOp<Binary::SET>(ret, setTo(ret, input));
		}));
	}
}

#endif