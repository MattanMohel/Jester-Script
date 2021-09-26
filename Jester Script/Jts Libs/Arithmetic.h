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
		env::AddSymbol(vm, "pi", env::AddConst<j_float>(M_PI));

		env::AddSymbol(vm, "rad2deg", env::AddConst<j_float>(180.0f/M_PI));
		
		env::AddSymbol(vm, "deg2rad", env::AddConst<j_float>(M_PI/180.0f));

		env::AddSymbol(vm, "+", env::AddNative([](Obj* ret, ObjNode* args, bool eval)
		{
			BinaryOpObj<BinaryOp::SET>(ret, EvalObj(args, eval));
			 
			while (args->next) 
			{
				BinaryOpObj<BinaryOp::ADD>(ret, EvalObj(args->next, eval));
				args = args->next;
			}

			return ret;
		}));


		env::AddSymbol(vm, "-", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			BinaryOpObj<BinaryOp::SET>(ret, EvalObj(args, eval));

			while (args->next)
			{
				BinaryOpObj<BinaryOp::SUB>(ret, EvalObj(args->next, eval));
				args = args->next;
			}

			return ret;
		}));

		env::AddSymbol(vm, "*", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			BinaryOpObj<BinaryOp::SET>(ret, EvalObj(args, eval));

			while (args->next)
			{
				BinaryOpObj<BinaryOp::MUL>(ret, EvalObj(args->next, eval));
				args = args->next;
			}

			return ret;
		}));

		env::AddSymbol(vm, "/", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			BinaryOpObj<BinaryOp::SET>(ret, EvalObj(args, eval));

			while (args->next)
			{
				BinaryOpObj<BinaryOp::DIV>(ret, EvalObj(args->next, eval));
				args = args->next;
			}

			return ret;
		}));		
		
		env::AddSymbol(vm, "%", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			BinaryOpObj<BinaryOp::SET>(ret, EvalObj(args, eval));

			while (args->next)
			{
				BinaryOpObj<BinaryOp::MOD>(ret, EvalObj(args->next, eval));
				args = args->next;
			}

			return ret;
		}));		
		
		env::AddSymbol(vm, "pow", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			BinaryOpObj<BinaryOp::SET>(ret, EvalObj(args, eval));

			while (args->next)
			{
				BinaryOpObj<BinaryOp::POW>(ret, EvalObj(args->next, eval));
				args = args->next;
			}

			return ret;
		}));		
		
		env::AddSymbol(vm, "+=", env::AddNative([](Obj* ret, ObjNode* args, bool eval)
		{
			auto* beg = args;

			while (args->next)
			{
				BinaryOpObj<BinaryOp::ADD>(EvalObj(beg, eval), EvalObj(args->next, eval));
				args = args->next;
			}

			return beg->value;
		}));

		env::AddSymbol(vm, "-=", env::AddNative([](Obj* ret, ObjNode* args, bool eval)
		{
			auto* beg = args;

			while (args->next)
			{
				BinaryOpObj<BinaryOp::SUB>(EvalObj(beg, eval), EvalObj(args->next, eval));
				args = args->next;
			}

			return beg->value;
		}));

		env::AddSymbol(vm, "*=", env::AddNative([](Obj* ret, ObjNode* args, bool eval)
		{
			auto* beg = args;

			while (args->next)
			{
				BinaryOpObj<BinaryOp::MUL>(EvalObj(beg, eval), EvalObj(args->next, eval));
				args = args->next;
			}

			return beg->value;
		}));

		env::AddSymbol(vm, "/=", env::AddNative([](Obj* ret, ObjNode* args, bool eval)
		{
			auto* beg = args;

			while (args->next)
			{
				BinaryOpObj<BinaryOp::DIV>(EvalObj(beg, eval), EvalObj(args->next, eval));
				args = args->next;
			}

			return beg->value;
		}));		
		
		env::AddSymbol(vm, "%=", env::AddNative([](Obj* ret, ObjNode* args, bool eval)
		{
			auto* beg = args;

			while (args->next)
			{
				BinaryOpObj<BinaryOp::MOD>(EvalObj(beg, eval), EvalObj(args->next, eval));
				args = args->next;
			}

			return beg->value;
		}));

		env::AddSymbol(vm, "++", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			return UnaryOpObj<UnaryOp::INCR>(EvalObj(args, eval));
		}));

		env::AddSymbol(vm, "--", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			return UnaryOpObj<UnaryOp::DECR>(EvalObj(args, eval));
		}));		
		
		env::AddSymbol(vm, "sin", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			BinaryOpObj<BinaryOp::SET>(ret, EvalObj(args, eval));
			return UnaryOpObj<UnaryOp::SIN>(ret);
		}));		
		
		env::AddSymbol(vm, "asin", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			BinaryOpObj<BinaryOp::SET>(ret, EvalObj(args, eval));
			return UnaryOpObj<UnaryOp::ASIN>(ret);
		}));
		
		env::AddSymbol(vm, "cos", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			BinaryOpObj<BinaryOp::SET>(ret, EvalObj(args, eval));
			return UnaryOpObj<UnaryOp::COS>(ret);
		}));
		
		env::AddSymbol(vm, "acos", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			BinaryOpObj<BinaryOp::SET>(ret, EvalObj(args, eval));
			return UnaryOpObj<UnaryOp::ACOS>(ret);
		}));
		
		env::AddSymbol(vm, "tan", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			BinaryOpObj<BinaryOp::SET>(ret, EvalObj(args, eval));
			return UnaryOpObj<UnaryOp::TAN>(ret);
		}));
		
		env::AddSymbol(vm, "atan", env::AddNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			BinaryOpObj<BinaryOp::SET>(ret, EvalObj(args, eval));
			return UnaryOpObj<UnaryOp::ATAN>(ret);
		}));
	}
}
#endif