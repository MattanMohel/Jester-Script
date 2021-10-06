#ifndef ARITHMETIC_H
#define ARITHMETIC_H

#include "../src/Object.h"
#include "../src/Operations.h"
#include "../src/Execute.h"
#include "../src/VM.h"

#define _USE_MATH_DEFINES

#include <math.h>
#include <limits>
#include <random>
#include <time.h>

using namespace jts;

namespace lib
{
	int Random(int min, int max)
	{
		std::random_device dev;
		std::mt19937 rng(dev());
		std::uniform_int_distribution<std::mt19937::result_type> dist(min, max);

		return dist(rng);
	}

	inline void ArithmeticLib(VM* vm)
	{
		// Mathematical Constants

		env::AddSymbol(vm, "pi", env::AddConst<j_float>(M_PI));

		env::AddSymbol(vm, "e", env::AddConst<j_float>(M_E));

		env::AddSymbol(vm, "epsilon", env::AddConst<j_float>(std::numeric_limits<j_float>::epsilon()));

		env::AddSymbol(vm, "rad2deg", env::AddConst<j_float>(180.0f/M_PI));
		
		env::AddSymbol(vm, "deg2rad", env::AddConst<j_float>(M_PI/180.0f));

		// Arithmetic Binary Operations

		env::AddSymbol(vm, "+", env::AddNative([](Obj* ret, ObjNode* args, bool eval)
		{
			BinaryOp<Binary::SET>(ret, EvalObj(args->value, eval));
			 
			while (args->next) 
			{
				BinaryOp<Binary::ADD>(ret, EvalObj(args->next->value, eval));
				args = args->next;
			}

			return ret;
		}));

		env::AddSymbol(vm, "-", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			BinaryOp<Binary::SET>(ret, EvalObj(args->value, eval));

			while (args->next)
			{
				BinaryOp<Binary::SUB>(ret, EvalObj(args->next->value, eval));
				args = args->next;
			}

			return ret;
		}));

		env::AddSymbol(vm, "*", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			BinaryOp<Binary::SET>(ret, EvalObj(args->value, eval));

			while (args->next)
			{
				BinaryOp<Binary::MUL>(ret, EvalObj(args->next->value, eval));
				args = args->next;
			}

			return ret;
		}));

		env::AddSymbol(vm, "/", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			BinaryOp<Binary::SET>(ret, EvalObj(args->value, eval));

			while (args->next)
			{
				BinaryOp<Binary::DIV>(ret, EvalObj(args->next->value, eval));
				args = args->next;
			}

			return ret;
		}));		
		
		env::AddSymbol(vm, "mod", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			BinaryOp<Binary::SET>(ret, EvalObj(args->value, eval));

			while (args->next)
			{
				BinaryOp<Binary::MOD>(ret, EvalObj(args->next->value, eval));
				args = args->next;
			}

			return ret;
		}));		
		
		env::AddSymbol(vm, "pow", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			BinaryOp<Binary::SET>(ret, EvalObj(args->value, eval));

			while (args->next)
			{
				BinaryOp<Binary::POW>(ret, EvalObj(args->next->value, eval));
				args = args->next;
			}

			return ret;
		}));					
		
		env::AddSymbol(vm, "log", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			BinaryOp<Binary::SET>(ret, EvalObj(args->value, eval));

			while (args->next)
			{
				BinaryOp<Binary::LOG>(ret, EvalObj(args->next->value, eval));
				args = args->next;
			}

			return ret;
		}));

		env::AddSymbol(vm, "+=", env::AddNative([](Obj* ret, ObjNode* args, bool eval)
		{
			ret = args->value;

			while (args->next)
			{
				BinaryOp<Binary::ADD>(EvalObj(ret, eval), EvalObj(args->next->value, eval));
				args = args->next;
			}

			return ret;
		}));

		env::AddSymbol(vm, "-=", env::AddNative([](Obj* ret, ObjNode* args, bool eval)
		{
			ret = args->value;

			while (args->next)
			{
				BinaryOp<Binary::SUB>(EvalObj(ret, eval), EvalObj(args->next->value, eval));
				args = args->next;
			}

			return ret;
		}));

		env::AddSymbol(vm, "*=", env::AddNative([](Obj* ret, ObjNode* args, bool eval)
		{
			ret = args->value;

			while (args->next)
			{
				BinaryOp<Binary::MUL>(EvalObj(ret, eval), EvalObj(args->next->value, eval));
				args = args->next;
			}

			return ret;
		}));

		env::AddSymbol(vm, "/=", env::AddNative([](Obj* ret, ObjNode* args, bool eval)
		{
			ret = args->value;

			while (args->next)
			{
				BinaryOp<Binary::DIV>(EvalObj(ret, eval), EvalObj(args->next->value, eval));
				args = args->next;
			}

			return ret;
		}));		
		
		env::AddSymbol(vm, "%=", env::AddNative([](Obj* ret, ObjNode* args, bool eval)
		{
			ret = args->value;

			while (args->next)
			{
				BinaryOp<Binary::MOD>(EvalObj(ret, eval), EvalObj(args->next->value, eval));
				args = args->next;
			}

			return ret;
		}));

		// Arithmetic Unary Operations
		
		env::AddSymbol(vm, "ln", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			BinaryOp<Binary::SET>(ret, EvalObj(args->value, eval));
			return UnaryOp<Unary::LN>(ret);
		}));		
		
		env::AddSymbol(vm, "++", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			return UnaryOp<Unary::INCR>(EvalObj(args->value, eval));
		}));

		env::AddSymbol(vm, "--", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			return UnaryOp<Unary::DECR>(EvalObj(args->value, eval));
		}));		
		
		env::AddSymbol(vm, "sin", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			BinaryOp<Binary::SET>(ret, EvalObj(args->value, eval));
			return UnaryOp<Unary::SIN>(ret);
		}));		
		
		env::AddSymbol(vm, "asin", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			BinaryOp<Binary::SET>(ret, EvalObj(args->value, eval));
			return UnaryOp<Unary::ASIN>(ret);
		}));
		
		env::AddSymbol(vm, "cos", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			BinaryOp<Binary::SET>(ret, EvalObj(args->value, eval));
			return UnaryOp<Unary::COS>(ret);
		}));
		
		env::AddSymbol(vm, "acos", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			BinaryOp<Binary::SET>(ret, EvalObj(args->value, eval));
			return UnaryOp<Unary::ACOS>(ret);
		}));
		
		env::AddSymbol(vm, "tan", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			BinaryOp<Binary::SET>(ret, EvalObj(args->value, eval));
			return UnaryOp<Unary::TAN>(ret);
		}));
		
		env::AddSymbol(vm, "atan", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			BinaryOp<Binary::SET>(ret, EvalObj(args->value, eval));
			return UnaryOp<Unary::ATAN>(ret);
		}));

		// (random min max)
		env::AddSymbol(vm, "random", env::AddBridge(Random));
	}
}
#endif