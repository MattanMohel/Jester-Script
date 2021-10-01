#ifndef ARITHMETIC_H
#define ARITHMETIC_H

#include "../src/Object.h"
#include "../src/Operations.h"
#include "../src/Execute.h"
#include "../src/VM.h"

#define _USE_MATH_DEFINES

#include <math.h>
#include <limits>

using namespace jts;

namespace lib
{
	inline void ArithmeticLib(VM* vm)
	{
		env::AddSymbol(vm, "pi", env::AddConst<j_float>(M_PI));

		env::AddSymbol(vm, "epsilon", env::AddConst<j_float>(std::numeric_limits<j_float>::epsilon()));

		env::AddSymbol(vm, "e", env::AddConst<j_float>(M_E));

		env::AddSymbol(vm, "rad2deg", env::AddConst<j_float>(180.0f/M_PI));
		
		env::AddSymbol(vm, "deg2rad", env::AddConst<j_float>(M_PI/180.0f));

		env::AddSymbol(vm, "+", env::AddNative([](Obj* ret, ObjNode* args, bool eval)
		{
			BinaryOp<Binary::SET>(ret, EvalObj(args, eval));
			 
			while (args->next) 
			{
				BinaryOp<Binary::ADD>(ret, EvalObj(args->next, eval));
				args = args->next;
			}

			return ret;
		}));


		env::AddSymbol(vm, "-", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			BinaryOp<Binary::SET>(ret, EvalObj(args, eval));

			while (args->next)
			{
				BinaryOp<Binary::SUB>(ret, EvalObj(args->next, eval));
				args = args->next;
			}

			return ret;
		}));

		env::AddSymbol(vm, "*", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			BinaryOp<Binary::SET>(ret, EvalObj(args, eval));

			while (args->next)
			{
				BinaryOp<Binary::MUL>(ret, EvalObj(args->next, eval));
				args = args->next;
			}

			return ret;
		}));

		env::AddSymbol(vm, "/", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			BinaryOp<Binary::SET>(ret, EvalObj(args, eval));

			while (args->next)
			{
				BinaryOp<Binary::DIV>(ret, EvalObj(args->next, eval));
				args = args->next;
			}

			return ret;
		}));		
		
		env::AddSymbol(vm, "mod", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			BinaryOp<Binary::SET>(ret, EvalObj(args, eval));

			while (args->next)
			{
				BinaryOp<Binary::MOD>(ret, EvalObj(args->next, eval));
				args = args->next;
			}

			return ret;
		}));		
		
		env::AddSymbol(vm, "pow", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			BinaryOp<Binary::SET>(ret, EvalObj(args, eval));

			while (args->next)
			{
				BinaryOp<Binary::POW>(ret, EvalObj(args->next, eval));
				args = args->next;
			}

			return ret;
		}));		
				
		
		env::AddSymbol(vm, "log", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			BinaryOp<Binary::SET>(ret, EvalObj(args, eval));

			while (args->next)
			{
				BinaryOp<Binary::LOG>(ret, EvalObj(args->next, eval));
				args = args->next;
			}

			return ret;
		}));			
		
		env::AddSymbol(vm, "ln", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			BinaryOp<Binary::SET>(ret, EvalObj(args, eval));
			return UnaryOp<Unary::LN>(ret);
		}));		
		
		env::AddSymbol(vm, "+=", env::AddNative([](Obj* ret, ObjNode* args, bool eval)
		{
			auto* beg = args;

			while (args->next)
			{
				BinaryOp<Binary::ADD>(EvalObj(beg, eval), EvalObj(args->next, eval));
				args = args->next;
			}

			return beg->value;
		}));

		env::AddSymbol(vm, "-=", env::AddNative([](Obj* ret, ObjNode* args, bool eval)
		{
			auto* beg = args;

			while (args->next)
			{
				BinaryOp<Binary::SUB>(EvalObj(beg, eval), EvalObj(args->next, eval));
				args = args->next;
			}

			return beg->value;
		}));

		env::AddSymbol(vm, "*=", env::AddNative([](Obj* ret, ObjNode* args, bool eval)
		{
			auto* beg = args;

			while (args->next)
			{
				BinaryOp<Binary::MUL>(EvalObj(beg, eval), EvalObj(args->next, eval));
				args = args->next;
			}

			return beg->value;
		}));

		env::AddSymbol(vm, "/=", env::AddNative([](Obj* ret, ObjNode* args, bool eval)
		{
			auto* beg = args;

			while (args->next)
			{
				BinaryOp<Binary::DIV>(EvalObj(beg, eval), EvalObj(args->next, eval));
				args = args->next;
			}

			return beg->value;
		}));		
		
		env::AddSymbol(vm, "%=", env::AddNative([](Obj* ret, ObjNode* args, bool eval)
		{
			auto* beg = args;

			while (args->next)
			{
				BinaryOp<Binary::MOD>(EvalObj(beg, eval), EvalObj(args->next, eval));
				args = args->next;
			}

			return beg->value;
		}));

		env::AddSymbol(vm, "++", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			return UnaryOp<Unary::INCR>(EvalObj(args, eval));
		}));

		env::AddSymbol(vm, "--", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			return UnaryOp<Unary::DECR>(EvalObj(args, eval));
		}));		
		
		env::AddSymbol(vm, "sin", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			BinaryOp<Binary::SET>(ret, EvalObj(args, eval));
			return UnaryOp<Unary::SIN>(ret);
		}));		
		
		env::AddSymbol(vm, "asin", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			BinaryOp<Binary::SET>(ret, EvalObj(args, eval));
			return UnaryOp<Unary::ASIN>(ret);
		}));
		
		env::AddSymbol(vm, "cos", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			BinaryOp<Binary::SET>(ret, EvalObj(args, eval));
			return UnaryOp<Unary::COS>(ret);
		}));
		
		env::AddSymbol(vm, "acos", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			BinaryOp<Binary::SET>(ret, EvalObj(args, eval));
			return UnaryOp<Unary::ACOS>(ret);
		}));
		
		env::AddSymbol(vm, "tan", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			BinaryOp<Binary::SET>(ret, EvalObj(args, eval));
			return UnaryOp<Unary::TAN>(ret);
		}));
		
		env::AddSymbol(vm, "atan", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			BinaryOp<Binary::SET>(ret, EvalObj(args, eval));
			return UnaryOp<Unary::ATAN>(ret);
		}));
	}
}
#endif