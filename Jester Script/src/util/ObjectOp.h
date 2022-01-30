#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "core/Types.h"
#include "core/VM.h"

namespace jts {

	////////////////////////
	/////Operator Enums/////
	////////////////////////

	enum class Binary {
		ADD,
		SUB,
		MUL,
		DIV,
		MOD,
		LOG,
		POW,
		ROOT,
	};

	enum class Unary {
		COS,
		SIN,
		ACOS,
		ASIN,
		TAN,
		ATAN,
		LN,
	};

	// Sets Obj to given integral type
	template<typename T>
	inline Obj* setTo(Obj* a, T value) {
		static_assert(false, "object type not supported");
	}

	// Preforms binary Obj operations
	template<Binary op>
	inline Obj* binaryOp(Obj* a, Obj* b) {
		static_assert(false, "binary operator not supported");
	}

	// Preforms unary Obj operations
	template<Unary op>
	inline Obj* unaryOp(Obj* a) {
		static_assert(false, "unary operator not supported");
	}


	/////////////////////////
	/////Implementations/////
	/////////////////////////

		/////////////////
		/////Setters/////
		/////////////////

	// Universal Object setter method
	Obj* setObj(VM* vm, Obj* a, Obj* b, bool canFree = true);

	template<> Obj* setTo<jtsc>(Obj* a, jtsc value);
	template<> Obj* setTo<jtsb>(Obj* a, jtsb value);
	template<> Obj* setTo<jtsi>(Obj* a, jtsi value);
	template<> Obj* setTo<jtsf>(Obj* a, jtsf value);
	template<> Obj* setTo<str*>(Obj* a, str* value);
	template<> Obj* setTo<nullptr_t>(Obj* a, nullptr_t value);

		///////////////////////////
		/////Binary Operations/////
		///////////////////////////

	template<> Obj* binaryOp<Binary::ADD>(Obj* a, Obj* b);
	template<> Obj* binaryOp<Binary::SUB>(Obj* a, Obj* b);
	template<> Obj* binaryOp<Binary::MUL>(Obj* a, Obj* b);
	template<> Obj* binaryOp<Binary::DIV>(Obj* a, Obj* b);
	template<> Obj* binaryOp<Binary::MOD>(Obj* a, Obj* b);
	template<> Obj* binaryOp<Binary::POW>(Obj* a, Obj* b);
	template<> Obj* binaryOp<Binary::LOG>(Obj* a, Obj* b);
	template<> Obj* binaryOp<Binary::ROOT>(Obj* a, Obj* b);

		//////////////////////////
		/////Unary Operations/////
		//////////////////////////

	template<> Obj* unaryOp<Unary::SIN> (Obj* a);
	template<> Obj* unaryOp<Unary::ASIN>(Obj* a);
	template<> Obj* unaryOp<Unary::COS> (Obj* a);
	template<> Obj* unaryOp<Unary::ACOS>(Obj* a);
	template<> Obj* unaryOp<Unary::TAN> (Obj* a);
	template<> Obj* unaryOp<Unary::ATAN>(Obj* a);
	template<> Obj* unaryOp<Unary::LN>  (Obj* a);

	////////////////////////////
	/////Boolean Operations/////
	////////////////////////////

	bool isTrue             (Obj* a);
	bool isEqual    (Obj* a, Obj* b);
	bool isGreater  (Obj* a, Obj* b);
	bool isGreaterEq(Obj* a, Obj* b);

	///////////////////////////
	/////Object Operations/////
	///////////////////////////

	Obj* quoteObj(VM* vm, Obj* q);
	void freeObj      (VM* vm, Obj* obj);

	int random(int min, int max);
}

#endif