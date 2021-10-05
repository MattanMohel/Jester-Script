#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "Types.h"

namespace jts
{
	enum class Binary
	{
		ADD,
		SUB,
		MUL,
		DIV,
		MOD,
		POW,
		LOG,

		SET,
	};

	enum class Unary
	{
		INCR,
		DECR,

		COS, 
		SIN,
		ACOS,
		ASIN,
		TAN,
		ATAN,
		LN,

		HASH,
		QUOTE,
	};

	// Preforms binary Obj operations
	template<Binary op>
	inline Obj* BinaryOp(Obj* a, Obj* b)
	{
		static_assert(false, "binary operator not supported");
	}

	// Preforms unary Obj operations
	template<Unary op>
	inline Obj* UnaryOp(Obj* a)
	{
		static_assert(false, "unary operator not supported");
	}

	// Sets Obj to given integral type
	template<typename T>
	inline Obj* SetTo(Obj* a, T value)
	{
		static_assert(false, "object type not supported");
	}

	// Implementations...
	
	template<> Obj* BinaryOp<Binary::ADD>(Obj* a, Obj* b);
	template<> Obj* BinaryOp<Binary::SUB>(Obj* a, Obj* b);
	template<> Obj* BinaryOp<Binary::MUL>(Obj* a, Obj* b);
	template<> Obj* BinaryOp<Binary::DIV>(Obj* a, Obj* b);
	template<> Obj* BinaryOp<Binary::MOD>(Obj* a, Obj* b);
	template<> Obj* BinaryOp<Binary::POW>(Obj* a, Obj* b);
	template<> Obj* BinaryOp<Binary::LOG>(Obj* a, Obj* b);
	template<> Obj* BinaryOp<Binary::SET>(Obj* a, Obj* b);

	template<> Obj* UnaryOp<Unary::QUOTE>(Obj* a);
	template<> Obj* UnaryOp<Unary::INCR>(Obj* a);
	template<> Obj* UnaryOp<Unary::DECR>(Obj* a);
	template<> Obj* UnaryOp<Unary::SIN>(Obj* a);
	template<> Obj* UnaryOp<Unary::COS>(Obj* a);
	template<> Obj* UnaryOp<Unary::ACOS>(Obj* a);
	template<> Obj* UnaryOp<Unary::ASIN>(Obj* a);
	template<> Obj* UnaryOp<Unary::TAN>(Obj* a);
	template<> Obj* UnaryOp<Unary::ATAN>(Obj* a);
	template<> Obj* UnaryOp<Unary::LN>(Obj* a);
	template<> Obj* UnaryOp<Unary::HASH>(Obj* a);

	template<> Obj* SetTo<j_char>(Obj* a, j_char value);
	template<> Obj* SetTo<j_bool>(Obj* a, j_bool value);
	template<> Obj* SetTo<j_int>(Obj* a, j_int value);
	template<> Obj* SetTo<j_float>(Obj* a, j_float value);
	template<> Obj* SetTo<str*>(Obj* a, str* value);
	template<> Obj* SetTo<nullptr_t>(Obj* a, nullptr_t value);

	// Boolean operations

	bool isTrue (Obj* a);
	bool isEqual(Obj* a, Obj* b);
	bool isGreater(Obj* a, Obj* b);
	bool isGreaterEq(Obj* a, Obj* b);
}

#endif
