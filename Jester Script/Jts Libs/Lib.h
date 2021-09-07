#ifndef STANDARD_H
#define STANDARD_H

#include "../src/VM.h"
#include "../src/Types.h"
#include "../src/Object.h"
#include "../src/Operations.h"
#include "../src/Execute.h"

#include <iostream>

using namespace jts;

inline void StdLib(VM* vm)
{
	env::AddNative(vm, "+", [](ObjNode* in)
	{
		BinaryOpObj<BinaryOp::SET>(in, in->args);
		auto* rest = in->args;

		while (rest->next)
		{
			BinaryOpObj<BinaryOp::ADD>(in, rest->next);
			rest = rest->next;
		}

		return in->value;
	});

	env::AddNative(vm, "-", [](ObjNode* in) -> Obj*
	{
		BinaryOpObj<BinaryOp::SET>(in, in->args);
		auto* rest = in->args;

		while (rest->next)
		{
			BinaryOpObj<BinaryOp::SUB>(in, rest->next);
			rest = rest->next;
		}

		return in->value;
	});	
	
	env::AddNative(vm, "*", [](ObjNode* in) -> Obj*
	{
		BinaryOpObj<BinaryOp::SET>(in, in->args);
		auto* rest = in->args;

		while (rest->next)
		{
			BinaryOpObj<BinaryOp::MUL>(in, rest->next);
			rest = rest->next;
		}

		return in->value;
	});	
	
	env::AddNative(vm, "/", [](ObjNode* in) -> Obj*
	{
		BinaryOpObj<BinaryOp::SET>(in, in->args);
		auto* rest = in->args;

		while (rest->next)
		{
			BinaryOpObj<BinaryOp::DIV>(in, rest->next);
			rest = rest->next;
		}

		return in->value;
	});	
	
	env::AddNative(vm, "++", [](ObjNode* in) -> Obj*
	{
		UnaryOpObj<UnaryOp::INCR>(in->args);

		return in->args->value;
	});		
	
	env::AddNative(vm, "--", [](ObjNode* in) -> Obj*
	{
		UnaryOpObj<UnaryOp::INCR>(in->args);

		return in->args->value;
	});	
	
	env::AddNative(vm, "set", [](ObjNode* args) -> Obj*
	{
		return BinaryOpObj<BinaryOp::SET>(args->args, args->args->next);
	});

	env::AddNative(vm, "print", [](ObjNode* in) -> Obj*
	{ 
		auto* rest = in->args;
		Obj* value = nullptr;

		while (rest)
		{
			value = EvalObj(rest);

			switch (value->type)
			{
				case Type::CHAR:

					std::cout << value->_char;
					break;

				case Type::BOOL:

					std::cout << (value->_bool? "true" : "false");
					break;

				case Type::INT:

					std::cout << value->_int;
					break;

				case Type::FLOAT:

					std::cout << value->_float;
					break;

				default: // case NIL
					std::cout << "nil";
					break;
			}

			rest = rest->next;
		}

		return value;
	});
	
	env::AddNative(vm, "println", [](ObjNode* args) -> Obj*
	{
		args = args->args;

		Obj* value = nullptr;

		while (args)
		{
			value = EvalObj(args);

			switch (value->type)
			{
				case Type::CHAR:

					std::cout << value->_char;
					break;

				case Type::BOOL:

					std::cout << (value->_bool ? "true" : "false");
					break;

				case Type::INT:

					std::cout << value->_int;
					break;

				case Type::FLOAT:

					std::cout << value->_float;
					break;

				default: // case NIL
					std::cout << "nil";
					break;
			}

			args = args->next;
		}

		std::cout << std::endl;

		return value;
	});
}

#endif