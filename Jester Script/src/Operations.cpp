#include <random>

#include "Operations.h"
#include "Execute.h"
#include "Object.h"
#include "Log.h"
#include "VM.h"

// char, int and bool share the same data -> for switch check if its float, otherwise operate on int --> transmutable for char bool & float

namespace jts {
	template<> Obj* binaryOp<Binary::ADD>(Obj* a, Obj* b) {
		switch (a->type) {

		case Type::FLOAT:

			a->_float += castObj<j_float>(b);
			break;

		default: // CHAR, BOOL, INT

			a->_int += castObj<j_int>(b);
			break;
		}

		return a;
	}

	template<> Obj* binaryOp<Binary::SUB>(Obj* a, Obj* b) {
		switch (a->type) {

		case Type::FLOAT:

			a->_float -= castObj<j_float>(b);
			break;

		default: // CHAR, BOOL, INT

			a->_int -= castObj<j_int>(b);
			break;
		}

		return a;
	}

	template<> Obj* binaryOp<Binary::MUL>(Obj* a, Obj* b) {
		switch (a->type) {

		case Type::FLOAT:

			a->_float *= castObj<j_float>(b);
			break;

		default: // CHAR, BOOL, INT

			a->_int *= castObj<j_int>(b);
			break;
		}

		return a;
	}

	template<> Obj* binaryOp<Binary::DIV>(Obj* a, Obj* b) {
		switch (a->type) {

		case Type::FLOAT:

			a->_float /= castObj<j_float>(b);
			break;

		default: // CHAR, BOOL, INT

			a->_int /= castObj<j_int>(b);
			break;
		}

		return a;
	}

	template<> Obj* binaryOp<Binary::MOD>(Obj* a, Obj* b) {
		switch (a->type) {

		case Type::FLOAT:

			a->_float = (j_int)a->_float % castObj<j_int>(b);
			break;

		default: // CHAR, BOOL, INT

			a->_int %= castObj<j_int>(b);
			break;
		}

		return a;
	}

	template<> Obj* binaryOp<Binary::POW>(Obj* a, Obj* b) {
		switch (a->type) {

		case Type::FLOAT:

			a->_float = pow(a->_float, castObj<j_float>(b));
			break;

		default: // CHAR, BOOL, INT

			a->_int = pow(a->_int, castObj<j_int>(b));
			break;
		}

		return a;
	}

	template<> Obj* binaryOp<Binary::ROOT>(Obj* a, Obj* b) {
		switch (a->type) {

		case Type::FLOAT:

			a->_float = pow(a->_float, 1.0 / castObj<j_float>(b));
			break;

		default: // CHAR, BOOL, INT

			a->_int = pow(a->_int, 1.0 / castObj<j_int>(b));
			break;
		}

		return a;
	}

	template<> Obj* binaryOp<Binary::LOG>(Obj* a, Obj* b) {
		switch (a->type) {

		case Type::FLOAT:

			a->_float = (j_float)(log(castObj<j_float>(b)) / log(castObj<j_float>(a)));
			break;

		default: // CHAR, BOOL, INT

			a->_int = (j_int)(log(castObj<j_float>(b)) / log(castObj<j_float>(a)));
			break;
		}

		return a;
	}

	template<> Obj* binaryOp<Binary::SET>(Obj* a, Obj* b) {
		// memory collection

		/*if (!isIntegral(a->type))
		{
		switch (a->type)
		{
		case Type::LIST:
		{
		auto elem = a->_args;

		while (elem)
		{
		env::releaseNode(elem);
		elem = elem->next;
		}

		break;
		}

		case Type::JTS_TYPE:
		{

		break;
		}

		case Type::CPP_TYPE:
		{

		break;
		}

		default:

		env::glbl_objPool.release(a);
		}
		}*/

	#if DEBUG
		a->assert(a->spec == Spec::VALUE || a->constant, "tried setting a constant value %");
	#endif 

		a->type = b->type;
		a->initialized = true;

		switch (b->type) {
		case Type::LIST:

			++b->refCount;

			if (b->spec == Spec::VALUE) {
				a->_args = copyList(b)->_args;
			}
			else {
				a->_args = b->_args;
			}

			break;

		case Type::NAT_FN:

			++b->refCount;
			a->_native = b->_native;
			break;

		case Type::JTS_FN:

			++b->refCount;
			a->_jtsFn = b->_jtsFn;
			break;

		case Type::MAC_FN:

			++b->refCount;
			a->_macFn = b->_macFn;
			break;

		case Type::CPP_FN:

			++b->refCount;
			a->_cppFn = b->_cppFn;
			break;

		case Type::QUOTE:

			++b->_quote->refCount;
			a->_quote = b->_quote;
			break;

		case Type::STRING:

			++b->refCount;
			a->_string = new str(*b->_string);
			break;

		case Type::FLOAT:

			a->_float = castObj<j_float>(b);
			break;

		default: // CHAR, BOOL, INT

			a->_int = castObj<j_int>(b);
			break;
		}

		return a;
	}

	template<> Obj* binarySet<Binary::ADD>(Obj* a, Obj* b) {
		return binaryOp<Binary::SET>(a, binaryOp<Binary::ADD>(a, b));
	}

	template<> Obj* binarySet<Binary::SUB>(Obj* a, Obj* b) {
		return binaryOp<Binary::SET>(a, binaryOp<Binary::SUB>(a, b));
	}

	template<> Obj* binarySet<Binary::MUL>(Obj* a, Obj* b) {
		return binaryOp<Binary::SET>(a, binaryOp<Binary::MUL>(a, b));
	}

	template<> Obj* binarySet<Binary::DIV>(Obj* a, Obj* b) {
		return binaryOp<Binary::SET>(a, binaryOp<Binary::DIV>(a, b));
	}

	template<> Obj* binarySet<Binary::MOD>(Obj* a, Obj* b) {
		return binaryOp<Binary::SET>(a, binaryOp<Binary::MOD>(a, b));
	}


	template<> Obj* unaryOp<Unary::INCR>(Obj* a) {
		switch (a->type) {
		case Type::FLOAT:

			++a->_float;
			break;

		default: // CHAR, BOOL, INT
			++a->_int;
			break;
		}

		return a;
	}

	template<> Obj* unaryOp<Unary::DECR>(Obj* a) {
		switch (a->type) {
		case Type::FLOAT:

			--a->_float;
			break;

		default: // CHAR, BOOL, INT
			--a->_int;
			break;
		}

		return a;
	}

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

			a->_float = (j_float)log(castObj<j_float>(a));
			break;

		default: // CHAR, BOOL, INT

			a->_int = (j_int)log(castObj<j_float>(a));
			break;
		}

		return a;
	}

	template<> Obj* unaryOp<Unary::HASH>(Obj* a) {
		switch (a->type) {
		case Type::INT:
		{
			std::hash<j_int> hash;
			a->_int = hash(a->_int);

			break;
		}

		case Type::FLOAT:
		{
			std::hash<j_float> hash;
			a->_int = hash(a->_float);

			break;
		}

		case Type::QUOTE:
		{
			std::hash<str> hash;
			a->_int = hash(a->_quote->symbol);

			break;
		}
		}

		a->type = Type::INT;

		return a;
	}

	template<> Obj* setTo<j_char>(Obj* a, j_char value) {
		a->type = Type::CHAR;
		a->_char = value;

		return a;
	}

	template<> Obj* setTo<j_bool>(Obj* a, j_bool value) {
		a->type = Type::BOOL;
		a->_bool = value;

		return a;
	}

	template<> Obj* setTo<j_int>(Obj* a, j_int value) {
		a->type = Type::INT;
		a->_int = value;

		return a;
	}

	template<> Obj* setTo<j_float>(Obj* a, j_float value) {
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

	bool isTrue(Obj* a) {
		switch (a->type) {
		case Type::FLOAT:

			return (j_bool)a->_float;

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

			return a->_float > castObj<j_float>(b);

		default: // CHAR, BOOL, INT

			return a->_int > castObj<j_int>(b);
		}

		return false;
	}

	bool isGreaterEq(Obj* a, Obj* b) {
		switch (a->type) {
		case Type::FLOAT:

			return a->_float >= castObj<j_float>(b);

		default: // CHAR, BOOL, INT

			return a->_int >= castObj<j_int>(b);
		}

		return false;
	}

	Obj* quoteObj(Obj* a, bool eval) {
		Obj* quote;

		// if quoting non-list item
		if (a->type != Type::LIST) {
			quote = env::glbl_objPool.acquire();

			quote->type = Type::QUOTE;
			quote->_quote = evalObj(a, eval);
			++a->refCount;

			return quote;
		}

		quote = copyList(a,
							  [&eval](Obj* obj)
		{
			return quoteObj(evalObj(obj, eval), eval);
		});

		return quote;
	}

	Obj* copyList(Obj* lst, std::function<Obj* (Obj*)> trans) {
		if (!lst->_args) {
			return nullptr;
		}

		auto elem = lst->_args;

		auto res = env::glbl_objPool.acquire();

		res->spec = Spec::VALUE;
		res->type = Type::LIST;

		auto lstPtr = &res->_args;

		while (elem) {
			(*lstPtr) = env::glbl_nodePool.acquire();
			(*lstPtr)->value = trans(elem->value);

			elem = elem->next;
			lstPtr = &(*lstPtr)->next;
		}

		return res;
	}


	Obj* copyList(ObjNode* lst, std::function<Obj* (Obj*)> trans) {
		if (!lst) {
			return nullptr;
		}

		auto elem = lst;

		auto res = env::glbl_objPool.acquire();

		res->spec = Spec::VALUE;
		res->type = Type::LIST;

		auto lstPtr = &res->_args;

		while (elem) {
			(*lstPtr) = env::glbl_nodePool.acquire();
			(*lstPtr)->value = trans(elem->value);

			elem = elem->next;
			lstPtr = &(*lstPtr)->next;
		}

		return res;
	}

	int random(int min, int max) {
		std::random_device dev;
		std::mt19937 rng(dev());
		std::uniform_int_distribution<std::mt19937::result_type> dist(min, max - 1);

		return dist(rng);
	}

}