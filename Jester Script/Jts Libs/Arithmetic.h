#ifndef ARITHMETIC_H
#define ARITHMETIC_H

#include "../src/Object.h"
#include "../src/Operations.h"
#include "../src/Execute.h"
#include "../src/VM.h"

#define _USE_MATH_DEFINES

#include <math.h>

using namespace jts;

namespace lib
{
	inline void ArithmeticLib(VM* vm)
	{
		env::AddSymbol(vm, "pi", env::AddConst<float>(M_PI));

		env::AddSymbol(vm, "rad2deg", env::AddConst<float>(180.0f/M_PI));
		
		env::AddSymbol(vm, "deg2rad", env::AddConst<float>(M_PI/180.0f));

		env::AddSymbol(vm, "+", env::AddNative([](ObjNode* fn, ObjNode* args)
		{
			BinaryOpObj<BinaryOp::SET>(fn, args);
			 
			while (args->next) 
			{
				BinaryOpObj<BinaryOp::ADD>(fn, args->next);
				args = args->next;
			}

			return fn->value;
		}));


		env::AddSymbol(vm, "-", env::AddNative([](ObjNode* fn, ObjNode* args) -> Obj*
		{
			BinaryOpObj<BinaryOp::SET>(fn, args);

			while (args->next)
			{
				BinaryOpObj<BinaryOp::SUB>(fn, args->next);
				args = args->next;
			}

			return fn->value;
		}));

		env::AddSymbol(vm, "*", env::AddNative([](ObjNode* fn, ObjNode* args) -> Obj*
		{
			BinaryOpObj<BinaryOp::SET>(fn, args);

			while (args->next)
			{
				BinaryOpObj<BinaryOp::MUL>(fn, args->next);
				args = args->next;
			}

			return fn->value;
		}));

		env::AddSymbol(vm, "/", env::AddNative([](ObjNode* fn, ObjNode* args) -> Obj*
		{
			BinaryOpObj<BinaryOp::SET>(fn, args);

			while (args->next)
			{
				BinaryOpObj<BinaryOp::DIV>(fn, args->next);
				args = args->next;
			}

			return fn->value;
		}));		
		
		env::AddSymbol(vm, "%", env::AddNative([](ObjNode* fn, ObjNode* args) -> Obj*
		{
			BinaryOpObj<BinaryOp::SET>(fn, args);

			while (args->next)
			{
				BinaryOpObj<BinaryOp::MOD>(fn, args->next);
				args = args->next;
			}

			return fn->value;
		}));

		env::AddSymbol(vm, "+=", env::AddNative([](ObjNode* fn, ObjNode* args)
		{
			auto* first = args;

			while (args->next)
			{
				BinaryOpObj<BinaryOp::ADD>(first, args->next);
				args = args->next;
			}

			return first->value;
		}));

		env::AddSymbol(vm, "-=", env::AddNative([](ObjNode* fn, ObjNode* args)
		{
			auto* first = args;

			while (args->next)
			{
				BinaryOpObj<BinaryOp::SUB>(first, args->next);
				args = args->next;
			}

			return first->value;
		}));

		env::AddSymbol(vm, "*=", env::AddNative([](ObjNode* fn, ObjNode* args)
		{
			auto* first = args;

			while (args->next)
			{
				BinaryOpObj<BinaryOp::MUL>(first, args->next);
				args = args->next;
			}

			return first->value;
		}));

		env::AddSymbol(vm, "/=", env::AddNative([](ObjNode* fn, ObjNode* args)
		{
			auto* first = args;

			while (args->next)
			{
				BinaryOpObj<BinaryOp::DIV>(first, args->next);
				args = args->next;
			}

			return first->value;
		}));		
		
		env::AddSymbol(vm, "%=", env::AddNative([](ObjNode* fn, ObjNode* args)
		{
			auto* first = args;

			while (args->next)
			{
				BinaryOpObj<BinaryOp::MOD>(first, args->next);
				args = args->next;
			}

			return first->value;
		}));

		env::AddSymbol(vm, "++", env::AddNative([](ObjNode* fn, ObjNode* args) -> Obj*
		{
			return UnaryOpObj<UnaryOp::INCR>(args);
		}));

		env::AddSymbol(vm, "--", env::AddNative([](ObjNode* fn, ObjNode* args) -> Obj*
		{
			return UnaryOpObj<UnaryOp::DECR>(args);
		}));		
		
		env::AddSymbol(vm, "sin", env::AddNative([](ObjNode* fn, ObjNode* args) -> Obj*
		{
			BinaryOpObj<BinaryOp::SET>(fn, args);
			return UnaryOpObj<UnaryOp::SIN>(args);
		}));		
		
		env::AddSymbol(vm, "asin", env::AddNative([](ObjNode* fn, ObjNode* args) -> Obj*
		{
			BinaryOpObj<BinaryOp::SET>(fn, args);
			return UnaryOpObj<UnaryOp::ASIN>(args);
		}));		
		
		env::AddSymbol(vm, "cos", env::AddNative([](ObjNode* fn, ObjNode* args) -> Obj*
		{
			BinaryOpObj<BinaryOp::SET>(fn, args);
			return UnaryOpObj<UnaryOp::COS>(args);
		}));		
		
		env::AddSymbol(vm, "acos", env::AddNative([](ObjNode* fn, ObjNode* args) -> Obj*
		{
			BinaryOpObj<BinaryOp::SET>(fn, args);
			return UnaryOpObj<UnaryOp::ACOS>(args);
		}));		
		
		env::AddSymbol(vm, "tan", env::AddNative([](ObjNode* fn, ObjNode* args) -> Obj*
		{
			BinaryOpObj<BinaryOp::SET>(fn, args);
			return UnaryOpObj<UnaryOp::TAN>(args);
		}));		
		
		env::AddSymbol(vm, "atan", env::AddNative([](ObjNode* fn, ObjNode* args) -> Obj*
		{
			BinaryOpObj<BinaryOp::SET>(fn, args);
			return UnaryOpObj<UnaryOp::ATAN>(args);
		}));
	}
}
#endif