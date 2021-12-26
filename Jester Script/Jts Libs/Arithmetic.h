#ifndef ARITHMETIC_H
#define ARITHMETIC_H

#include "../src/Types.h"
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

namespace lib {
	inline void ArithmeticLib(VM* vm) {
		// Mathematical Constants

		env::addSymbol(vm, "pi", env::addConst<j_float>(M_PI));

		env::addSymbol(vm, "e", env::addConst<j_float>(M_E));

		env::addSymbol(vm, "epsilon", env::addConst<j_float>(std::numeric_limits<j_float>::epsilon()));

		env::addSymbol(vm, "rad2deg", env::addConst<j_float>(180.0f / M_PI));

		env::addSymbol(vm, "deg2rad", env::addConst<j_float>(M_PI / 180.0f));

		// Miscellaneous

		// (random min max)
		env::addSymbol(vm, "random", env::addFunction(random));

		// Arithmetic Binary Operations

		env::addSymbol(vm, "+", env::addNative([](Obj* ret, ObjNode* args, bool eval)
		{
			binaryOp<Binary::SET>(ret, evalObj(args->value, eval));

			while (args->next) {
				binaryOp<Binary::ADD>(ret, evalObj(args->next->value, eval));
				args = args->next;
			}

			return ret;
		}));

		env::addSymbol(vm, "-", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			binaryOp<Binary::SET>(ret, evalObj(args->value, eval));

			while (args->next) {
				binaryOp<Binary::SUB>(ret, evalObj(args->next->value, eval));
				args = args->next;
			}

			return ret;
		}));

		env::addSymbol(vm, "*", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			binaryOp<Binary::SET>(ret, evalObj(args->value, eval));

			while (args->next) {
				binaryOp<Binary::MUL>(ret, evalObj(args->next->value, eval));
				args = args->next;
			}

			return ret;
		}));

		env::addSymbol(vm, "/", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			binaryOp<Binary::SET>(ret, evalObj(args->value, eval));

			while (args->next) {
				binaryOp<Binary::DIV>(ret, evalObj(args->next->value, eval));
				args = args->next;
			}

			return ret;
		}));

		env::addSymbol(vm, "mod", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			binaryOp<Binary::SET>(ret, evalObj(args->value, eval));

			while (args->next) {
				binaryOp<Binary::MOD>(ret, evalObj(args->next->value, eval));
				args = args->next;
			}

			return ret;
		}));

		env::addSymbol(vm, "pow", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			binaryOp<Binary::SET>(ret, evalObj(args->value, eval));

			while (args->next) {
				binaryOp<Binary::POW>(ret, evalObj(args->next->value, eval));
				args = args->next;
			}

			return ret;
		}));

		env::addSymbol(vm, "root", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			binaryOp<Binary::SET>(ret, evalObj(args->value, eval));

			while (args->next) {
				binaryOp<Binary::ROOT>(ret, evalObj(args->next->value, eval));
				args = args->next;
			}

			return ret;
		}));

		env::addSymbol(vm, "log", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			binaryOp<Binary::SET>(ret, evalObj(args->value, eval));

			while (args->next) {
				binaryOp<Binary::LOG>(ret, evalObj(args->next->value, eval));
				args = args->next;
			}

			return ret;
		}));

		env::addSymbol(vm, "+=", env::addNative([](Obj* ret, ObjNode* args, bool eval)
		{
			ret = evalObj(args->value, eval);

			while (args->next) {
				binarySet<Binary::ADD>(ret, evalObj(args->next->value, eval));
				args = args->next;
			}

			return ret;
		}));

		env::addSymbol(vm, "-=", env::addNative([](Obj* ret, ObjNode* args, bool eval)
		{
			ret = evalObj(args->value, eval);

			while (args->next) {
				binarySet<Binary::SUB>(ret, evalObj(args->next->value, eval));
				args = args->next;
			}

			return ret;
		}));

		env::addSymbol(vm, "*=", env::addNative([](Obj* ret, ObjNode* args, bool eval)
		{
			ret = evalObj(args->value, eval);

			while (args->next) {
				binarySet<Binary::MUL>(ret, evalObj(args->next->value, eval));
				args = args->next;
			}

			return ret;
		}));

		env::addSymbol(vm, "/=", env::addNative([](Obj* ret, ObjNode* args, bool eval)
		{
			ret = evalObj(args->value, eval);

			while (args->next) {
				binarySet<Binary::DIV>(ret, evalObj(args->next->value, eval));
				args = args->next;
			}

			return ret;
		}));

		env::addSymbol(vm, "%=", env::addNative([](Obj* ret, ObjNode* args, bool eval)
		{
			ret = evalObj(args->value, eval);

			while (args->next) {
				binarySet<Binary::MOD>(ret, evalObj(args->next->value, eval));
				args = args->next;
			}

			return ret;
		}));

		// Arithmetic Unary Operations

		env::addSymbol(vm, "ln", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			binaryOp<Binary::SET>(ret, evalObj(args->value, eval));
			return unaryOp<Unary::LN>(ret);
		}));

		/*		env::addSymbol(vm, "incr", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
				{
					return unaryOp<Unary::INCR>(evalObj(args->value, eval));
				}));

				env::addSymbol(vm, "decr", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
				{
					return unaryOp<Unary::DECR>(evalObj(args->value, eval));
				}));	*/

		env::addSymbol(vm, "sin", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			binaryOp<Binary::SET>(ret, evalObj(args->value, eval));
			return unaryOp<Unary::SIN>(ret);
		}));

		env::addSymbol(vm, "asin", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			binaryOp<Binary::SET>(ret, evalObj(args->value, eval));
			return unaryOp<Unary::ASIN>(ret);
		}));

		env::addSymbol(vm, "cos", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			binaryOp<Binary::SET>(ret, evalObj(args->value, eval));
			return unaryOp<Unary::COS>(ret);
		}));

		env::addSymbol(vm, "acos", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			binaryOp<Binary::SET>(ret, evalObj(args->value, eval));
			return unaryOp<Unary::ACOS>(ret);
		}));

		env::addSymbol(vm, "tan", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			binaryOp<Binary::SET>(ret, evalObj(args->value, eval));
			return unaryOp<Unary::TAN>(ret);
		}));

		env::addSymbol(vm, "atan", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			binaryOp<Binary::SET>(ret, evalObj(args->value, eval));
			return unaryOp<Unary::ATAN>(ret);
		}));
	}
}
#endif