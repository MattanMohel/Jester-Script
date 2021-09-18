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
	inline Obj* BinaryOpObj(ObjNode* a, ObjNode* b)
	{
		static_assert(false, "binary operator not supported");
	}

	template<UnaryOp op>
	inline Obj* UnaryOpObj(ObjNode* a)
	{
		static_assert(false, "unary operator not supported");
	}	
	
	template<> Obj* BinaryOpObj<BinaryOp::ADD>(ObjNode* a, ObjNode* b);
	template<> Obj* BinaryOpObj<BinaryOp::SUB>(ObjNode* a, ObjNode* b);
	template<> Obj* BinaryOpObj<BinaryOp::MUL>(ObjNode* a, ObjNode* b);
	template<> Obj* BinaryOpObj<BinaryOp::DIV>(ObjNode* a, ObjNode* b);
	template<> Obj* BinaryOpObj<BinaryOp::MOD>(ObjNode* a, ObjNode* b);
	template<> Obj* BinaryOpObj<BinaryOp::SET>(ObjNode* a, ObjNode* b);

	template<> Obj* UnaryOpObj<UnaryOp::INCR>(ObjNode* a);
	template<> Obj* UnaryOpObj<UnaryOp::DECR>(ObjNode* a);
	
	template<> Obj* UnaryOpObj<UnaryOp::SIN>(ObjNode* a);
	template<> Obj* UnaryOpObj<UnaryOp::ASIN>(ObjNode* a);
	template<> Obj* UnaryOpObj<UnaryOp::COS>(ObjNode* a);
	template<> Obj* UnaryOpObj<UnaryOp::ACOS>(ObjNode* a);
	template<> Obj* UnaryOpObj<UnaryOp::TAN>(ObjNode* a);
	template<> Obj* UnaryOpObj<UnaryOp::ATAN>(ObjNode* a);

	bool isTrue (ObjNode* a);
	bool isEqual(ObjNode* a, ObjNode* b);
	bool isGreater(ObjNode* a, ObjNode* b);
	bool isGreaterEq(ObjNode* a, ObjNode* b);
	Obj* SetState(ObjNode* a, bool state);

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
