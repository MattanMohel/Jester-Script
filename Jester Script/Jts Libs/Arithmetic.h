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

		env::AddSymbol(vm, "+", env::AddNative([](ObjNode* in)
		{
			BinaryOpObj<BinaryOp::SET>(in, in->args);
			auto* rest = in->args;

			while (rest->next)
			{
				BinaryOpObj<BinaryOp::ADD>(in, rest->next);
				rest = rest->next;
			}

			return in->value;
		}));


		env::AddSymbol(vm, "-", env::AddNative([](ObjNode* in) -> Obj*
		{
			BinaryOpObj<BinaryOp::SET>(in, in->args);
			auto* rest = in->args;

			while (rest->next)
			{
				BinaryOpObj<BinaryOp::SUB>(in, rest->next);
				rest = rest->next;
			}

			return in->value;
		}));

		env::AddSymbol(vm, "*", env::AddNative([](ObjNode* in) -> Obj*
		{
			BinaryOpObj<BinaryOp::SET>(in, in->args);
			auto* rest = in->args;

			while (rest->next)
			{
				BinaryOpObj<BinaryOp::MUL>(in, rest->next);
				rest = rest->next;
			}

			return in->value;
		}));

		env::AddSymbol(vm, "/", env::AddNative([](ObjNode* in) -> Obj*
		{
			BinaryOpObj<BinaryOp::SET>(in, in->args);
			auto* rest = in->args;

			while (rest->next)
			{
				BinaryOpObj<BinaryOp::DIV>(in, rest->next);
				rest = rest->next;
			}

			return in->value;
		}));		
		
		env::AddSymbol(vm, "%", env::AddNative([](ObjNode* in) -> Obj*
		{
			BinaryOpObj<BinaryOp::SET>(in, in->args);
			auto* rest = in->args;

			while (rest->next)
			{
				BinaryOpObj<BinaryOp::MOD>(in, rest->next);
				rest = rest->next;
			}

			return in->value;
		}));

		env::AddSymbol(vm, "+=", env::AddNative([](ObjNode* in)
		{
			auto* rest = in->args;

			while (rest->next)
			{
				BinaryOpObj<BinaryOp::ADD>(in->args, rest->next);
				rest = rest->next;
			}

			return in->args->value;
		}));

		env::AddSymbol(vm, "-=", env::AddNative([](ObjNode* in)
		{
			auto* rest = in->args;

			while (rest->next)
			{
				BinaryOpObj<BinaryOp::SUB>(in->args, rest->next);
				rest = rest->next;
			}

			return in->args->value;
		}));

		env::AddSymbol(vm, "*=", env::AddNative([](ObjNode* in)
		{
			auto* rest = in->args;

			while (rest->next)
			{
				BinaryOpObj<BinaryOp::MUL>(in->args, rest->next);
				rest = rest->next;
			}

			return in->args->value;
		}));

		env::AddSymbol(vm, "/=", env::AddNative([](ObjNode* in)
		{
			auto* rest = in->args;

			while (rest->next)
			{
				BinaryOpObj<BinaryOp::DIV>(in->args, rest->next);
				rest = rest->next;
			}

			return in->args->value;
		}));		
		
		env::AddSymbol(vm, "%=", env::AddNative([](ObjNode* in)
		{
			auto* rest = in->args;

			while (rest->next)
			{
				BinaryOpObj<BinaryOp::MOD>(in->args, rest->next);
				rest = rest->next;
			}

			return in->args->value;
		}));

		env::AddSymbol(vm, "++", env::AddNative([](ObjNode* in) -> Obj*
		{
			return UnaryOpObj<UnaryOp::INCR>(in->args);
		}));

		env::AddSymbol(vm, "--", env::AddNative([](ObjNode* in) -> Obj*
		{
			return UnaryOpObj<UnaryOp::DECR>(in->args);
		}));		
		
		env::AddSymbol(vm, "sin", env::AddNative([](ObjNode* in) -> Obj*
		{
			BinaryOpObj<BinaryOp::SET>(in, in->args);
			return UnaryOpObj<UnaryOp::SIN>(in->args);
		}));		
		
		env::AddSymbol(vm, "asin", env::AddNative([](ObjNode* in) -> Obj*
		{
			BinaryOpObj<BinaryOp::SET>(in, in->args);
			return UnaryOpObj<UnaryOp::ASIN>(in->args);
		}));		
		
		env::AddSymbol(vm, "cos", env::AddNative([](ObjNode* in) -> Obj*
		{
			BinaryOpObj<BinaryOp::SET>(in, in->args);
			return UnaryOpObj<UnaryOp::COS>(in->args);
		}));		
		
		env::AddSymbol(vm, "acos", env::AddNative([](ObjNode* in) -> Obj*
		{
			BinaryOpObj<BinaryOp::SET>(in, in->args);
			return UnaryOpObj<UnaryOp::ACOS>(in->args);
		}));		
		
		env::AddSymbol(vm, "tan", env::AddNative([](ObjNode* in) -> Obj*
		{
			BinaryOpObj<BinaryOp::SET>(in, in->args);
			return UnaryOpObj<UnaryOp::TAN>(in->args);
		}));		
		
		env::AddSymbol(vm, "atan", env::AddNative([](ObjNode* in) -> Obj*
		{
			BinaryOpObj<BinaryOp::SET>(in, in->args);
			return UnaryOpObj<UnaryOp::ATAN>(in->args);
		}));
	}
}
#endif