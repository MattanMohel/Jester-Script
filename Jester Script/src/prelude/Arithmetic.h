#ifndef ARITHMETIC_H
#define ARITHMETIC_H

#include "core/Types.h"
#include "core/Object.h"
#include "core/Execute.h"
#include "core/VM.h"

#include "util/ObjectOp.h"

#define _USE_MATH_DEFINES

#include <math.h>
#include <limits>
#include <random>
#include <time.h>

using namespace jts;

namespace lib {
	inline void ArithmeticLib(VM* vm) {
		// Mathematical Constants

		env::addSymbol(vm, "pi", env::addConst<jtsf>(M_PI));

		env::addSymbol(vm, "e", env::addConst<jtsf>(M_E));

		env::addSymbol(vm, "epsilon", env::addConst<jtsf>(std::numeric_limits<jtsf>::epsilon()));

		env::addSymbol(vm, "rad2deg", env::addConst<jtsf>(180.0f / M_PI));

		env::addSymbol(vm, "deg2rad", env::addConst<jtsf>(M_PI / 180.0f));

		// Miscellaneous

		// (random min max)
		env::addSymbol(vm, "random", env::addFunction(random));

		// Arithmetic Binary Operations

		env::addSymbol(vm, "+", env::addNative([](VM* vm, Node* args)
		{
			auto ret = env::newObj(vm);
			setObj(vm, ret, evalObj(vm, args->val));

			while (args->nxt) {
				binaryOp<Binary::ADD>(ret, evalObj(vm, args->nxt->val));
				args = args->nxt;
			}

			return ret;
		}));

		env::addSymbol(vm, "-", env::addNative([](VM* vm, Node* args) 
		{
			auto ret = env::newObj(vm);
			setObj(vm, ret, evalObj(vm, args->val));

			while (args->nxt) {
				binaryOp<Binary::SUB>(ret, evalObj(vm, args->nxt->val));
				args = args->nxt;
			}

			return ret;
		}));

		env::addSymbol(vm, "*", env::addNative([](VM* vm, Node* args) 
		{
			auto ret = env::newObj(vm);
			setObj(vm, ret, evalObj(vm, args->val));

			while (args->nxt) {
				binaryOp<Binary::MUL>(ret, evalObj(vm, args->nxt->val));
				args = args->nxt;
			}

			return ret;
		}));

		env::addSymbol(vm, "/", env::addNative([](VM* vm, Node* args) 
		{
			auto ret = env::newObj(vm);
			setObj(vm, ret, evalObj(vm, args->val));

			while (args->nxt) {
				binaryOp<Binary::DIV>(ret, evalObj(vm, args->nxt->val));
				args = args->nxt;
			}

			return ret;
		}));

		env::addSymbol(vm, "mod", env::addNative([](VM* vm, Node* args) 
		{
			auto ret = env::newObj(vm);
			setObj(vm, ret, evalObj(vm, args->val));

			while (args->nxt) {
				binaryOp<Binary::MOD>(ret, evalObj(vm, args->nxt->val));
				args = args->nxt;
			}

			return ret;
		}));

		env::addSymbol(vm, "pow", env::addNative([](VM* vm, Node* args) 
		{
			auto ret = env::newObj(vm);
			setObj(vm, ret, evalObj(vm, args->val));

			while (args->nxt) {
				binaryOp<Binary::POW>(ret, evalObj(vm, args->nxt->val));
				args = args->nxt;
			}

			return ret;
		}));

		env::addSymbol(vm, "root", env::addNative([](VM* vm, Node* args) 
		{
			auto ret = env::newObj(vm);
			setObj(vm, ret, evalObj(vm, args->val));

			while (args->nxt) {
				binaryOp<Binary::ROOT>(ret, evalObj(vm, args->nxt->val));
				args = args->nxt;
			}

			return ret;
		}));

		env::addSymbol(vm, "log", env::addNative([](VM* vm, Node* args) 
		{
			auto ret = env::newObj(vm);
			setObj(vm, ret, evalObj(vm, args->val));

			while (args->nxt) {
				binaryOp<Binary::LOG>(ret, evalObj(vm, args->nxt->val));
				args = args->nxt;
			}

			return ret;
		}));

		env::addSymbol(vm, "+=", env::addNative([](VM* vm, Node* args)
		{
			auto val = args->val;

			while (args->nxt) {
				setObj(vm, val, binaryOp<Binary::ADD>(val, evalObj(vm, args->nxt->val)));
				args = args->nxt;
			}

			return setObj(vm, env::newObj(vm), val);
		}));

		env::addSymbol(vm, "-=", env::addNative([](VM* vm, Node* args)
		{
			auto val = args->val;

			while (args->nxt) {
				setObj(vm, val, binaryOp<Binary::SUB>(val, evalObj(vm, args->nxt->val)));
				args = args->nxt;
			}

			return setObj(vm, env::newObj(vm), val);
		}));

		env::addSymbol(vm, "*=", env::addNative([](VM* vm, Node* args)
		{
			auto val = args->val;

			while (args->nxt) {
				setObj(vm, val, binaryOp<Binary::MUL>(val, evalObj(vm, args->nxt->val)));
				args = args->nxt;
			}

			return setObj(vm, env::newObj(vm), val);
		}));

		env::addSymbol(vm, "/=", env::addNative([](VM* vm, Node* args)
		{
			auto val = args->val;

			while (args->nxt) {
				setObj(vm, val, binaryOp<Binary::DIV>(val, evalObj(vm, args->nxt->val)));
				args = args->nxt;
			}

			return setObj(vm, env::newObj(vm), val);
		}));

		env::addSymbol(vm, "%=", env::addNative([](VM* vm, Node* args)
		{
			auto val = args->val;

			while (args->nxt) {
				setObj(vm, val, binaryOp<Binary::MOD>(val, evalObj(vm, args->nxt->val)));
				args = args->nxt;
			}

			return setObj(vm, env::newObj(vm), val);
		}));

		// Arithmetic Unary Operations

		env::addSymbol(vm, "ln", env::addNative([](VM* vm, Node* args) 
		{
			auto ret = setObj(vm, env::newObj(vm), evalObj(vm, args->val));
			return unaryOp<Unary::LN>(ret);
		}));

		env::addSymbol(vm, "sin", env::addNative([](VM* vm, Node* args) 
		{
			auto ret = setObj(vm, env::newObj(vm), evalObj(vm, args->val));
			return unaryOp<Unary::SIN>(ret);
		}));

		env::addSymbol(vm, "asin", env::addNative([](VM* vm, Node* args) 
		{
			auto ret = setObj(vm, env::newObj(vm), evalObj(vm, args->val));
			return unaryOp<Unary::ASIN>(ret);
		}));

		env::addSymbol(vm, "cos", env::addNative([](VM* vm, Node* args) 
		{
			auto ret = setObj(vm, env::newObj(vm), evalObj(vm, args->val));
			return unaryOp<Unary::COS>(ret);
		}));

		env::addSymbol(vm, "acos", env::addNative([](VM* vm, Node* args) 
		{
			auto ret = setObj(vm, env::newObj(vm), evalObj(vm, args->val));
			return unaryOp<Unary::ACOS>(ret);
		}));

		env::addSymbol(vm, "tan", env::addNative([](VM* vm, Node* args) 
		{
			auto ret = setObj(vm, env::newObj(vm), evalObj(vm, args->val));
			return unaryOp<Unary::TAN>(ret);
		}));

		env::addSymbol(vm, "atan", env::addNative([](VM* vm, Node* args) 
		{
			auto ret = setObj(vm, env::newObj(vm), evalObj(vm, args->val));
			return unaryOp<Unary::ATAN>(ret);
		}));
	}
}
#endif