#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "Types.h"
#include "Object.h"

namespace jts
{
	enum class BinaryOp
	{
		ADD,
		SUB,
		MUL,
		DIV,
		MOD,		

		SET,
		EQUAL,
	};

	enum class UnaryOp
	{
		INCR,
		DECR,

		NOT,
	};

	template<BinaryOp op>
	inline Obj* BinaryOpObj(Obj* a, Obj* b)
	{
		static_assert(false, "binary operator not supported");
	}

	template<UnaryOp op>
	inline Obj* UnaryOpObj(Obj* a)
	{
		static_assert(false, "unary operator not supported");
	}

	template<> Obj* BinaryOpObj<BinaryOp::ADD>(Obj* a, Obj* b);
	template<> Obj* BinaryOpObj<BinaryOp::SUB>(Obj* a, Obj* b);
	template<> Obj* BinaryOpObj<BinaryOp::SET>(Obj* a, Obj* b);

	template<typename T>
	inline T CastObj(Obj* obj)
	{
		switch (obj->type)
		{
			case Type::CHAR:

				return static_cast<T>(obj->_char);
			
			case Type::BOOL:

				return static_cast<T>(obj->_bool);

			case Type::INT:

				return static_cast<T>(obj->_int);

			default: // case FLOAT

				return static_cast<T>(obj->_float);
		}
	}

	template<typename T>
	inline Type GetType()
	{
		if (std::is_same<T, char>::value)
		{
			return Type::INT;
		}
		else if (std::is_same<T, bool>::value)
		{
			return Type::BOOL;
		}
		else if (std::is_same<T, int>::value)
		{
			return Type::INT;
		}
		else if (std::is_same<T, float>::value)
		{
			return Type::FLOAT;
		}
	}
}

#endif
