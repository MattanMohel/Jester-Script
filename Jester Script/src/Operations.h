#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "Types.h"
#include "VM.h"

namespace jts {

	enum class Binary {
		ADD,
		SUB,
		MUL,
		DIV,
		MOD,
		LOG,
		POW,
		ROOT,
		COPY,
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

	// Sets Obj to given integral type
	template<typename T>
	inline Obj* setTo(Obj* a, T value) {
		static_assert(false, "object type not supported");
	}

	// Universal Object setter method
	Obj* setObj(VM* vm, Obj* a, Obj* b, bool canFree = true);

	/////////////////////////
	/////Implementations/////
	/////////////////////////

	template<> Obj* binaryOp<Binary::ADD>(Obj* a, Obj* b);
	template<> Obj* binaryOp<Binary::SUB>(Obj* a, Obj* b);
	template<> Obj* binaryOp<Binary::MUL>(Obj* a, Obj* b);
	template<> Obj* binaryOp<Binary::DIV>(Obj* a, Obj* b);
	template<> Obj* binaryOp<Binary::MOD>(Obj* a, Obj* b);
	template<> Obj* binaryOp<Binary::POW>(Obj* a, Obj* b);
	template<> Obj* binaryOp<Binary::LOG>(Obj* a, Obj* b);
	template<> Obj* binaryOp<Binary::ROOT>(Obj* a, Obj* b);

	template<> Obj* unaryOp<Unary::SIN> (Obj* a);
	template<> Obj* unaryOp<Unary::ASIN>(Obj* a);
	template<> Obj* unaryOp<Unary::COS> (Obj* a);
	template<> Obj* unaryOp<Unary::ACOS>(Obj* a);
	template<> Obj* unaryOp<Unary::TAN> (Obj* a);
	template<> Obj* unaryOp<Unary::ATAN>(Obj* a);
	template<> Obj* unaryOp<Unary::LN>  (Obj* a);


	template<> Obj* setTo<jtsc>(Obj* a, jtsc value);
	template<> Obj* setTo<jtsb>(Obj* a, jtsb value);
	template<> Obj* setTo<jtsi>(Obj* a, jtsi value);
	template<> Obj* setTo<jtsf>(Obj* a, jtsf value);
	template<> Obj* setTo<str*>(Obj* a, str* value);
	template<> Obj* setTo<nullptr_t>(Obj* a, nullptr_t value);

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

	Obj* quoteObj(VM* vm, Obj* res, Obj* q);
	void freeObj         (VM* vm, Obj* obj);
	
	// Iterates over each node of list and copies the
	// transformed result from the 'copy' lambda to a new list
	Node* listCpy(VM* vm, Node* lst, 
		std::function<Obj* (VM*, Obj*)> copy = [](VM* vm, Obj* obj) { 
			return setObj(vm, env::newObj(vm), obj);
	});	

	void listForEach(VM* vm, Node* lst, std::function<void(VM*, Node*)> forEach);

	int random(int min, int max);
}

#endif