#ifndef STANDARD_H
#define STANDARD_H

#include "../src/VM.h"
#include "../src/Types.h"
#include "../src/Object.h"
#include "../src/Operations.h"
#include "../src/Execute.h"

#include <iostream>

using namespace jts;

inline void StandardLib(VM* vm)
{
	env::AddNative(vm, "+", [](Obj* args)
	{
		BinaryOpObj<BinaryOp::SET>(args, args->args);
		Obj* rest = args->args;

		while (rest)
		{
			BinaryOpObj<BinaryOp::ADD>(args, rest);
			rest = rest->args;
		}

		return args;
	});

	env::AddNative(vm, "-", [](Obj* args) -> Obj*
	{
		BinaryOpObj<BinaryOp::SET>(args, args->args);
		Obj* rest = args->args;

		while (rest)
		{
			BinaryOpObj<BinaryOp::SUB>(args, rest);
			rest = rest->args;
		}

		return args;
	});

	env::AddNative(vm, "set", [](Obj* args) -> Obj*
	{
		return BinaryOpObj<BinaryOp::SET>(args, args->args);
	});

	env::AddNative(vm, "print", [](Obj* args) -> Obj*
	{
		Obj* rest = args->args;

		while (rest)
		{
			switch (rest->type)
			{
				case Type::CHAR:

					std::cout << EvalObj(rest)->_char;
					break;

				case Type::BOOL:

					std::cout << EvalObj(rest)->_bool;
					break;

				case Type::INT:

					std::cout << EvalObj(rest)->_int;
					break;

				case Type::FLOAT:

					std::cout << EvalObj(rest)->_float;
					break;
			}

			rest = rest->next;
		}

		return rest;
	});
}

#endif