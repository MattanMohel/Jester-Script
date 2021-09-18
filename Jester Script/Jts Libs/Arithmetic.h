#ifndef ARITHMETIC_H
#define ARITHMETIC_H

#include "../src/Object.h"
#include "../src/Operations.h"
#include "../src/Execute.h"
#include "../src/VM.h"

using namespace jts;

namespace lib
{
	inline void ArithmeticLib(VM* vm)
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

		env::AddNative(vm, "+=", [](ObjNode* in)
		{
			auto* rest = in->args;

			while (rest->next)
			{
				BinaryOpObj<BinaryOp::ADD>(in->args, rest->next);
				rest = rest->next;
			}

			return in->args->value;
		});

		env::AddNative(vm, "-=", [](ObjNode* in)
		{
			auto* rest = in->args;

			while (rest->next)
			{
				BinaryOpObj<BinaryOp::SUB>(in->args, rest->next);
				rest = rest->next;
			}

			return in->args->value;
		});

		env::AddNative(vm, "*=", [](ObjNode* in)
		{
			auto* rest = in->args;

			while (rest->next)
			{
				BinaryOpObj<BinaryOp::MUL>(in->args, rest->next);
				rest = rest->next;
			}

			return in->args->value;
		});

		env::AddNative(vm, "/=", [](ObjNode* in)
		{
			auto* rest = in->args;

			while (rest->next)
			{
				BinaryOpObj<BinaryOp::DIV>(in->args, rest->next);
				rest = rest->next;
			}

			return in->args->value;
		});

		env::AddNative(vm, "++", [](ObjNode* in) -> Obj*
		{
			return UnaryOpObj<UnaryOp::INCR>(in->args);
		});

		env::AddNative(vm, "--", [](ObjNode* in) -> Obj*
		{
			return UnaryOpObj<UnaryOp::DECR>(in->args);
		});
	}
}
#endif