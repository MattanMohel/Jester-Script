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
	};

	// Preforms binary Obj operations
	template<Binary op>
	inline Obj* binaryOp(Obj* a, Obj* b)
	{
		static_assert(false, "binary operator not supported");
	}

	// Preforms unary Obj operations
	template<Unary op>
	inline Obj* unaryOp(Obj* a)
	{
		static_assert(false, "unary operator not supported");
	}

	// Sets Obj to given integral type
	template<typename T>
	inline Obj* setTo(Obj* a, T value)
	{
		static_assert(false, "object type not supported");
	}

	// Implementations...
	
	template<> Obj* binaryOp<Binary::ADD>(Obj* a, Obj* b);
	template<> Obj* binaryOp<Binary::SUB>(Obj* a, Obj* b);
	template<> Obj* binaryOp<Binary::MUL>(Obj* a, Obj* b);
	template<> Obj* binaryOp<Binary::DIV>(Obj* a, Obj* b);
	template<> Obj* binaryOp<Binary::MOD>(Obj* a, Obj* b);
	template<> Obj* binaryOp<Binary::POW>(Obj* a, Obj* b);
	template<> Obj* binaryOp<Binary::LOG>(Obj* a, Obj* b);
	template<> Obj* binaryOp<Binary::SET>(Obj* a, Obj* b);

	template<> Obj* unaryOp<Unary::INCR>(Obj* a);
	template<> Obj* unaryOp<Unary::DECR>(Obj* a);
	template<> Obj* unaryOp<Unary::SIN>(Obj* a);
	template<> Obj* unaryOp<Unary::COS>(Obj* a);
	template<> Obj* unaryOp<Unary::ACOS>(Obj* a);
	template<> Obj* unaryOp<Unary::ASIN>(Obj* a);
	template<> Obj* unaryOp<Unary::TAN>(Obj* a);
	template<> Obj* unaryOp<Unary::ATAN>(Obj* a);
	template<> Obj* unaryOp<Unary::LN>(Obj* a);
	template<> Obj* unaryOp<Unary::HASH>(Obj* a);

	Obj* quoteObj(ObjNode* a, bool eval);

	template<> Obj* setTo<j_char>(Obj* a, j_char value);
	template<> Obj* setTo<j_bool>(Obj* a, j_bool value);
	template<> Obj* setTo<j_int>(Obj* a, j_int value);
	template<> Obj* setTo<j_float>(Obj* a, j_float value);
	template<> Obj* setTo<str*>(Obj* a, str* value);
	template<> Obj* setTo<nullptr_t>(Obj* a, nullptr_t value);

	// Boolean operations

	bool isTrue (Obj* a);
	bool isEqual(Obj* a, Obj* b);
	bool isGreater(Obj* a, Obj* b);
	bool isGreaterEq(Obj* a, Obj* b);
}

#endif
