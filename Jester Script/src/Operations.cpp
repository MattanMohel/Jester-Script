#include <random>

#include "Operations.h"
#include "Execute.h"
#include "Object.h"
#include "Log.h"
#include "VM.h"

// char, int and bool share the same data -> for switch check if its float, otherwise operate on int --> transmutable for char bool & float

namespace jts {

	Obj* setObj(VM* vm, Obj* a, Obj* b, bool canFree) {

	#if VALIDATE
		a->assert(a->spec == Spec::VALUE || a->constant, "tried setting a constant value %");
	#endif 

		// memory collection
		if (canFree && a->refCount) {
			if (*a->refCount < 2 && !isIntegral(a->type)) {
				freeObj(vm, a);
			}
			else {
				--(*a->refCount);
			}
		}

		if (!b->refCount && !isIntegral(b->type)) {
			b->refCount = new size_t(0);
		}

		a->type = b->type;
		a->initialized = true;

		switch (b->type) {
		case Type::LIST:

			if (b->spec == Spec::VALUE) {
				a->_args = listCpy(vm, b->_args);
			}
			else {
				a->_args = b->_args;
			}

			++(*b->refCount);

			break;

		case Type::NAT_FN:

			++(*b->refCount);
			a->_native = b->_native;
			break;

		case Type::JTS_FN:

			++(*b->refCount);
			a->_jtsFn = b->_jtsFn;
			break;

		case Type::CPP_FN:

			++(*b->refCount);
			a->_cppFn = b->_cppFn;
			break;

		case Type::QUOTE:

			++(*b->_quote->refCount);
			a->_quote = b->_quote;
			break;

		case Type::STRING:

			++(*b->refCount);
			a->_string = new str(*b->_string);
			break;

		case Type::FLOAT:

			a->_float = castObj<jtsf>(b);
			break;

		default: // CHAR, BOOL, INT

			a->_int = castObj<jtsi>(b);
			break;
		}

		a->refCount = b->refCount;

		return a;
	}

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

	Obj* quoteObj(VM* vm, Obj* res, Obj* q) {
		// if quoting non-list item
		if (q->type != Type::LIST) {
			res->type = Type::QUOTE;
			res->_quote = evalObj(vm, q);

			if (q->refCount) {
				++q->refCount;
			}

			return res;
		}

		res->type = Type::LIST;

		res->_args = listCpy(vm, q->_args,
			[](VM* vm, Obj* obj) {
				return quoteObj(vm, env::newObj(vm), evalObj(vm, obj));
		});

		return res;
	}

	void freeObj(VM* vm, Obj* obj) {
	
	#if DEBUG_ALLOC
			std::cout << "freeing " << obj->symbol << " - " << obj << '\n';
	#endif

		delete obj->refCount;

		switch (obj->type) {

		case Type::JTS_FN:

			delete obj->_jtsFn;
			return;

		case Type::CPP_FN:

			delete obj->_cppFn;
			return;

		case Type::QUOTE:

			if ((*obj->_quote->refCount) <= 1 && !isIntegral(obj->type)) {
				env::releaseObj(vm, obj->_quote);
				freeObj(vm, obj->_quote);
			}

			return;

		case Type::STRING:

			delete obj->_string;
			return;

		case Type::LIST: 

			listForEach(vm, obj->_args,
				[](VM* vm, Node* node) {
					env::releaseNode(vm, node);
					
					if (*node->val->refCount <= 1 && !isIntegral(node->val->type)) {
						freeObj(vm, node->val);
					}
			});
		} 
	}

	Node* listCpy(VM* vm, Node* lst, std::function<Obj* (VM*, Obj*)> copy) {
		if (!lst) {
			return nullptr;
		}

		Node* res = nullptr;
		Node** resPtr = &res;

		while (lst) {
			(*resPtr) = env::newNode(vm, copy(vm, lst->val));

			lst = lst->nxt;
			resPtr = &(*resPtr)->nxt;
		}

		return res;
	}

	void listForEach(VM* vm, Node* lst, std::function<void(VM*, Node*)> forEach) {
		if (!lst) {
			return;
		}

		while (lst) {
			forEach(vm, lst);
			lst = lst->nxt;
		}
	}

	int random(int min, int max) {
		std::random_device dev;
		std::mt19937 rng(dev());
		std::uniform_int_distribution<std::mt19937::result_type> dist(min, max - 1);

		return dist(rng);
	}

}