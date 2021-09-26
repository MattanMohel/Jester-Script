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
		POW,

		SET,
	};

	enum class UnaryOp
	{
		INCR,
		DECR,

		COS, 
		SIN,
		ACOS,
		ASIN,
		TAN,
		ATAN,
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
	template<> Obj* BinaryOpObj<BinaryOp::MUL>(Obj* a, Obj* b);
	template<> Obj* BinaryOpObj<BinaryOp::DIV>(Obj* a, Obj* b);
	template<> Obj* BinaryOpObj<BinaryOp::MOD>(Obj* a, Obj* b);
	template<> Obj* BinaryOpObj<BinaryOp::POW>(Obj* a, Obj* b);

	template<> Obj* BinaryOpObj<BinaryOp::SET>(Obj* a, Obj* b);

	template<> Obj* UnaryOpObj<UnaryOp::INCR>(Obj* a);
	template<> Obj* UnaryOpObj<UnaryOp::DECR>(Obj* a);
	
	template<> Obj* UnaryOpObj<UnaryOp::SIN>(Obj* a);
	template<> Obj* UnaryOpObj<UnaryOp::COS>(Obj* a);
	template<> Obj* UnaryOpObj<UnaryOp::ACOS>(Obj* a);
	template<> Obj* UnaryOpObj<UnaryOp::ASIN>(Obj* a);
	template<> Obj* UnaryOpObj<UnaryOp::TAN>(Obj* a);
	template<> Obj* UnaryOpObj<UnaryOp::ATAN>(Obj* a);

	bool isTrue (Obj* a);
	bool isEqual(Obj* a, Obj* b);
	bool isGreater(Obj* a, Obj* b);
	bool isGreaterEq(Obj* a, Obj* b);
	Obj* SetState(Obj* a, bool state);

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
