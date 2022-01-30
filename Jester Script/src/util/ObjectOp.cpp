#include <random>

#include "ObjectOp.h"
#include "ListOp.h"

#include "core/Execute.h"
#include "core/Object.h"
#include "core/Log.h"
#include "core/VM.h"

// char, int and bool share the same data,
// default case sets the data simultaneously 
// for all three types

namespace jts {

	/////////////////
	/////Setters/////
	/////////////////

	Obj* setObj(VM* vm, Obj* a, Obj* b, bool canFree) {

		ASSERT(a->spec == Spec::VALUE || a->constant, "tried setting a constant value");

		if (canFree) {
			freeObj(vm, a);

			if (b->spec == Spec::SYMBOL) {
				a->ref = b->ref;
				++(*b->ref);
			}
			else if (b->spec == Spec::VALUE) {
				*(a->ref) = 1;
			}
		}

		a->type = b->type;
		a->initialized = true;

		switch (b->type) {
			case Type::LIST:

				if (b->spec == Spec::VALUE) {
					a->_args = lst::copy(vm, b->_args);

				}
				else {
					a->_args = b->_args;
				}

				break;

			case Type::QUOTE:

				a->_quote = b->_quote;

				break;

			case Type::STRING:

				a->_string = b->_string;

				break;

			case Type::NAT_FN:

				a->_natFn = b->_natFn;

				break;

			case Type::JTS_FN:

				a->_jtsFn = b->_jtsFn;

				break;

			case Type::CPP_FN:

				a->_cppFn = b->_cppFn;

				break;

			case Type::FLOAT:

				a->_float = castObj<jtsf>(b);
				break;

			default: // CHAR, BOOL, INT

				a->_int = castObj<jtsi>(b);
				break;
		}

		return a;
	}

	template<> Obj* setTo<jtsc>(Obj* a, jtsc value) {
		a->type = Type::CHAR;
		a->_char = value;

		return a;
	}

	template<> Obj* setTo<jtsb>(Obj* a, jtsb value) {
		a->type = Type::BOOL;
		a->_bool = value;

		return a;
	}

	template<> Obj* setTo<jtsi>(Obj* a, jtsi value) {
		a->type = Type::INT;
		a->_int = value;

		return a;
	}

	template<> Obj* setTo<jtsf>(Obj* a, jtsf value) {
		a->type = Type::FLOAT;
		a->_float = value;

		return a;
	}

	template<> Obj* setTo<str*>(Obj* a, str* value) {
		a->type = Type::STRING;
		a->_string = value;

		return a;
	}

	template<> Obj* setTo<nullptr_t>(Obj* a, nullptr_t value) {
		a->type = Type::NIL;
		a->_int = 0;

		return a;
	}

	///////////////////////////
	/////Binary Operations/////
	///////////////////////////

	template<> Obj* binaryOp<Binary::ADD>(Obj* a, Obj* b) {
		switch (a->type) {

			case Type::FLOAT:

				a->_float += castObj<jtsf>(b);
				break;

			default: // CHAR, BOOL, INT

				a->_int += castObj<jtsi>(b);
				break;
		}

		return a;
	}

	template<> Obj* binaryOp<Binary::SUB>(Obj* a, Obj* b) {
		switch (a->type) {

			case Type::FLOAT:

				a->_float -= castObj<jtsf>(b);
				break;

			default: // CHAR, BOOL, INT

				a->_int -= castObj<jtsi>(b);
				break;
		}

		return a;
	}

	template<> Obj* binaryOp<Binary::MUL>(Obj* a, Obj* b) {
		switch (a->type) {

			case Type::FLOAT:

				a->_float *= castObj<jtsf>(b);
				break;

			default: // CHAR, BOOL, INT

				a->_int *= castObj<jtsi>(b);
				break;
		}

		return a;
	}

	template<> Obj* binaryOp<Binary::DIV>(Obj* a, Obj* b) {
		switch (a->type) {

			case Type::FLOAT:

				a->_float /= castObj<jtsf>(b);
				break;

			default: // CHAR, BOOL, INT

				a->_int /= castObj<jtsi>(b);
				break;
		}

		return a;
	}

	template<> Obj* binaryOp<Binary::MOD>(Obj* a, Obj* b) {
		switch (a->type) {

			case Type::FLOAT:

				a->_float = (jtsi)a->_float % castObj<jtsi>(b);
				break;

			default: // CHAR, BOOL, INT

				a->_int %= castObj<jtsi>(b);
				break;
		}

		return a;
	}

	template<> Obj* binaryOp<Binary::POW>(Obj* a, Obj* b) {
		switch (a->type) {

			case Type::FLOAT:

				a->_float = pow(a->_float, castObj<jtsf>(b));
				break;

			default: // CHAR, BOOL, INT

				a->_int = pow(a->_int, castObj<jtsi>(b));
				break;
		}

		return a;
	}

	template<> Obj* binaryOp<Binary::LOG>(Obj* a, Obj* b) {
		switch (a->type) {

			case Type::FLOAT:

				a->_float = (jtsf)(log(castObj<jtsf>(b)) / log(castObj<jtsf>(a)));
				break;

			default: // CHAR, BOOL, INT

				a->_int = (jtsi)(log(castObj<jtsf>(b)) / log(castObj<jtsf>(a)));
				break;
		}

		return a;
	}

	template<> Obj* binaryOp<Binary::ROOT>(Obj* a, Obj* b) {
		switch (a->type) {

			case Type::FLOAT:

				a->_float = pow(a->_float, 1.0 / castObj<jtsf>(b));
				break;

			default: // CHAR, BOOL, INT

				a->_int = pow(a->_int, 1.0 / castObj<jtsi>(b));
				break;
		}

		return a;
	}

	//////////////////////////
	/////Unary Operations/////
	//////////////////////////

	template<> Obj* unaryOp<Unary::SIN>(Obj* a) {
		switch (a->type) {
			case Type::FLOAT:

				a->_float = sin(a->_float);
				break;

			default: // CHAR, BOOL, INT

				a->_int = sin(a->_int);
				break;
		}

		return a;
	}

	template<> Obj* unaryOp<Unary::ASIN>(Obj* a) {
		switch (a->type) {
			case Type::FLOAT:

				a->_float = asin(a->_float);
				break;

			default: // CHAR, BOOL, INT

				a->_int = asin(a->_int);
				break;
		}

		return a;
	}

	template<> Obj* unaryOp<Unary::COS>(Obj* a) {
		switch (a->type) {
			case Type::FLOAT:

				a->_float = cos(a->_float);
				break;

			default: // CHAR, BOOL, INT

				a->_int = cos(a->_int);
				break;
		}

		return a;
	}

	template<> Obj* unaryOp<Unary::ACOS>(Obj* a) {
		switch (a->type) {
			case Type::FLOAT:

				a->_float = acos(a->_float);
				break;

			default: // CHAR, BOOL, INT

				a->_int = acos(a->_int);
				break;
		}

		return a;
	}

	template<> Obj* unaryOp<Unary::TAN>(Obj* a) {
		switch (a->type) {
			case Type::FLOAT:

				a->_float = tan(a->_float);
				break;

			default: // CHAR, BOOL, INT

				a->_int = tan(a->_int);
				break;
		}

		return a;
	}

	template<> Obj* unaryOp<Unary::ATAN>(Obj* a) {
		switch (a->type) {
			case Type::FLOAT:

				a->_float = atan(a->_float);
				break;

			default: // CHAR, BOOL, INT

				a->_int = atan(a->_int);
				break;
		}

		return a;
	}

	template<> Obj* unaryOp<Unary::LN>(Obj* a) {
		switch (a->type) {
			case Type::FLOAT:

				a->_float = (jtsf)log(castObj<jtsf>(a));
				break;

			default: // CHAR, BOOL, INT

				a->_int = (jtsi)log(castObj<jtsf>(a));
				break;
		}

		return a;
	}

	////////////////////////////
	/////Boolean Operations/////
	////////////////////////////

	bool isTrue(Obj* a) {
		switch (a->type) {
			case Type::FLOAT:

				return (jtsb)a->_float;

			default: // CHAR, BOOL, INT

				return a->_bool;
		}
	}

	bool isEqual(Obj* a, Obj* b) {
		switch (a->type) {
			case Type::FLOAT:

				return a->_float == b->_float;

			default: // CHAR, BOOL, INT

				return a->_int == b->_int;
		}
	}

	bool isGreater(Obj* a, Obj* b) {
		switch (a->type) {
			case Type::FLOAT:

				return a->_float > castObj<jtsf>(b);

			default: // CHAR, BOOL, INT

				return a->_int > castObj<jtsi>(b);
		}

		return false;
	}

	bool isGreaterEq(Obj* a, Obj* b) {
		switch (a->type) {
			case Type::FLOAT:

				return a->_float >= castObj<jtsf>(b);

			default: // CHAR, BOOL, INT

				return a->_int >= castObj<jtsi>(b);
		}

		return false;
	}

	///////////////////////////
	/////Object Operations/////
	///////////////////////////

	Obj* quoteObj(VM* vm, Obj* q) {
		auto res = env::newObj(vm);

		// if quoting non-list item
		if (q->type != Type::LIST) {

			res->type = Type::QUOTE;
			res->_quote = evalObj<true>(vm, q);

			return res;
		}

		res->type = Type::LIST;

		res->_args = lst::copy(vm, q->_args,
			[](VM* vm, Obj* obj) {
			return quoteObj(vm, evalObj<true>(vm, obj));
		});

		return res;
	}

	void freeObj(VM* vm, Obj* obj) {
		size_t* ref = (obj->type == Type::QUOTE)
			? obj->_quote->ref
			: obj->ref;		

		if (!ref) {
			return;
		}

		(*ref) -= 1;

		if (*ref > 0) {
			return;
		}

		switch (obj->type) {
			case Type::LIST:

				lst::free(vm, obj->_args);

				break;

			case Type::QUOTE:

				freeObj(vm, obj->_quote);

				break;

			case Type::STRING:

				delete obj->_string;

				break;

			case Type::CPP_FN:

				//TODO: implement destructor
				delete obj->_cppFn;

				break;

			case Type::NAT_FN:

				delete obj->_natFn;

				break;

			case Type::JTS_FN:

				// TODO: implement destructor
				delete obj->_jtsFn;

				break;
		}
	}

	int random(int min, int max) {
		std::random_device dev;
		std::mt19937 rng(dev());
		std::uniform_int_distribution<std::mt19937::result_type> dist(min, max - 1);

		return dist(rng);
	}
}