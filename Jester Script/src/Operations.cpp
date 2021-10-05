#include "Operations.h"
#include "Execute.h"
#include "Object.h"
#include "VM.h"

// char, int and bool share the same data -> for switch check if its float, otherwise operate on int --> transmutable for char bool & float

namespace jts
{
	template<> Obj* BinaryOp<Binary::ADD>(Obj* a, Obj* b)
	{
		switch (a->type)
		{
			case Type::FLOAT:

				a->_float += CastObj<j_float>(b);
				break;
			
			default: // CHAR, BOOL, INT
				
				a->_int += CastObj<j_int>(b);
				break;
		}

		return a;
	}

	template<> Obj* BinaryOp<Binary::SUB>(Obj* a, Obj* b)
	{
		switch (a->type)
		{
			case Type::FLOAT:

				a->_float -= CastObj<j_float>(b);
				break;

			default: // CHAR, BOOL, INT
				
				a->_int -= CastObj<j_int>(b);
				break;
		}

		return a;
	}		
	
	template<> Obj* BinaryOp<Binary::MUL>(Obj* a, Obj* b)
	{
		switch (a->type)
		{
			case Type::FLOAT:

				a->_float *= CastObj<j_float>(b);
				break;

			default: // CHAR, BOOL, INT
				
				a->_int *= CastObj<j_int>(b);
				break;
		}

		return a;
	}		
	
	template<> Obj* BinaryOp<Binary::DIV>(Obj* a, Obj* b)
	{
		switch (a->type)
		{
			case Type::FLOAT:

				a->_float /= CastObj<j_float>(b);
				break;

			default: // CHAR, BOOL, INT
				
				a->_int /= CastObj<j_int>(b);
				break;
		}

		return a;
	}	

	template<> Obj* BinaryOp<Binary::MOD>(Obj* a, Obj* b)
	{
		switch (a->type)
		{
			case Type::FLOAT:

				a->_float = (j_int)a->_float % CastObj<j_int>(b);
				break;

			default: // CHAR, BOOL, INT
				
				a->_int %= CastObj<j_int>(b);
				break;
		}

		return a;
	}

	template<> Obj* BinaryOp<Binary::POW>(Obj* a, Obj* b)
	{
		switch (a->type)
		{
			case Type::FLOAT:

				a->_float = pow(a->_float, CastObj<j_float>(b));
				break;

			default: // CHAR, BOOL, INT

				a->_int = pow(a->_int, CastObj<j_int>(b));
				break;
		}

		return a;
	}	
	
	template<> Obj* BinaryOp<Binary::LOG>(Obj* a, Obj* b)
	{
		switch (a->type)
		{
			case Type::FLOAT:

				a->_float = (j_float)(log(CastObj<j_float>(b)) / log(CastObj<j_float>(a)));
				break;

			default: // CHAR, BOOL, INT

				a->_int = (j_int)(log(CastObj<j_float>(b)) / log(CastObj<j_float>(a)));
				break;
		}

		return a;
	}
	
	template<> Obj* BinaryOp<Binary::SET>(Obj* a, Obj* b)
	{
		// memory collection -- need reference counting first

		//if (a->type == Type::LIST)
		//{
		//	auto* elem = a->_args;

		//	while (elem)
		//	{
		//		env::ReleaseNode(elem);
		//		elem = elem->next;
		//	}
		//}

		a->type = b->type;

		switch (b->type)
		{
			case Type::LIST:

				a->_args = b->_args;

				break;

			case Type::NAT_FN:

				a->_native = b->_native;
				break;

			case Type::JTS_FN:

				a->_jtsFunc = b->_jtsFunc;
				break;

			case Type::CPP_FN:
				
				a->_cppFunc = b->_cppFunc;
				break;

			case Type::QUOTE:

				a->_quote = b->_quote;
				break;

			case Type::STRING:

				a->_string = b->_string;
				break;

			case Type::FLOAT:

				a->_float = CastObj<j_float>(b);
				break;

			default: // CHAR, BOOL, INT

				a->_int = CastObj<j_int>(b);
				break;
		}

		return a;
	}
	
	template<> Obj* UnaryOp<Unary::QUOTE>(Obj* a)
	{
		Obj* quote = env::glbl_objPool.acquire();

		// if quoting non-list item

		if (a->type != Type::LIST)
		{
			quote->type = Type::QUOTE;
			quote->_quote = a;

			return quote;
		}

		// else if item is list...

		quote->type = Type::LIST;

		quote->_args = env::AcquireNode();
		quote->_args->value = UnaryOp<Unary::QUOTE>(a->_args->value);

		auto* elem = a->_args;
		auto* cell = quote->_args;

		while (elem->next)
		{
			cell->next = env::AcquireNode();
			cell->next->value = UnaryOp<Unary::QUOTE>(elem->next->value);

			cell = cell->next;
			elem = elem->next;
		}

		return quote;
	}

	template<> Obj* UnaryOp<Unary::INCR>(Obj* a)
	{
		switch (a->type)
		{
			case Type::FLOAT:

				++a->_float;
				break;

			default: // CHAR, BOOL, INT
				++a->_int;
				break;
		}

		return a;
	}	
	
	template<> Obj* UnaryOp<Unary::DECR>(Obj* a)
	{
		switch (a->type)
		{
			case Type::FLOAT:

				--a->_float;
				break;

			default: // CHAR, BOOL, INT
				--a->_int;
				break;
		}

		return a;
	}

	template<> Obj* UnaryOp<Unary::SIN>(Obj* a)
	{
		switch (a->type)
		{
			case Type::FLOAT:

				a->_float = sin(a->_float);
				break;

			default: // CHAR, BOOL, INT
				
				a->_int = sin(a->_int);
				break;
		}

		return a;
	}

	template<> Obj* UnaryOp<Unary::ASIN>(Obj* a)
	{
		switch (a->type)
		{
			case Type::FLOAT:

				a->_float = asin(a->_float);
				break;

			default: // CHAR, BOOL, INT

				a->_int = asin(a->_int);
				break;
		}

		return a;
	}

	template<> Obj* UnaryOp<Unary::COS>(Obj* a)
	{
		switch (a->type)
		{
			case Type::FLOAT:

				a->_float = cos(a->_float);
				break;

			default: // CHAR, BOOL, INT

				a->_int = cos(a->_int);
				break;
		}

		return a;
	}

	template<> Obj* UnaryOp<Unary::ACOS>(Obj* a)
	{
		switch (a->type)
		{
			case Type::FLOAT:

				a->_float = acos(a->_float);
				break;

			default: // CHAR, BOOL, INT

				a->_int = acos(a->_int);
				break;
		}

		return a;
	}

	template<> Obj* UnaryOp<Unary::TAN>(Obj* a)
	{
		switch (a->type)
		{
			case Type::FLOAT:

				a->_float = tan(a->_float);
				break;

			default: // CHAR, BOOL, INT

				a->_int = tan(a->_int);
				break;
		}

		return a;
	}

	template<> Obj* UnaryOp<Unary::ATAN>(Obj* a)
	{
		switch (a->type)
		{
			case Type::FLOAT:

				a->_float = atan(a->_float);
				break;

			default: // CHAR, BOOL, INT

				a->_int = atan(a->_int);
				break;
		}

		return a;
	}

	template<> Obj* UnaryOp<Unary::LN>(Obj* a)
	{
		switch (a->type)
		{
			case Type::FLOAT:

				a->_float = (j_float)log(CastObj<j_float>(a));
				break;

			default: // CHAR, BOOL, INT

				a->_int = (j_int)log(CastObj<j_float>(a));
				break;
		}

		return a;
	}

	template<> Obj* UnaryOp<Unary::HASH>(Obj* a)
	{
		switch (a->type) 
		{
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

	template<> Obj* SetTo<j_char>(Obj* a, j_char value)
	{
		a->type = Type::CHAR;
		a->_char = value;

		return a;
	}

	template<> Obj* SetTo<j_bool>(Obj* a, j_bool value)
	{
		a->type = Type::BOOL;
		a->_bool = value;

		return a;
	}

	template<> Obj* SetTo<j_int>(Obj* a, j_int value)
	{
		a->type = Type::INT;
		a->_int = value;

		return a;
	}

	template<> Obj* SetTo<j_float>(Obj* a, j_float value)
	{
		a->type = Type::FLOAT;
		a->_float = value;

		return a;
	}

	template<> Obj* SetTo<str*>(Obj* a, str* value)
	{
		a->type = Type::STRING;
		a->_string = value;

		return a;
	}

	template<> Obj* SetTo<nullptr_t>(Obj* a, nullptr_t value)
	{
		a->type = Type::NIL;
		a->_int = 0;

		return a;
	}

	bool isTrue(Obj* a)
	{
		switch (a->type)
		{
			case Type::FLOAT:

				return (j_bool)a->_float;
				break;

			default: // CHAR, BOOL, INT

				return a->_bool;
				break;
		}
	}

	bool isEqual(Obj* a, Obj* b)
	{
		switch (a->type)
		{
			case Type::FLOAT:

				return a->_float == b->_float;

			default: // CHAR, BOOL, INT

				return a->_int == b->_int;
		}
	}

	bool isGreater(Obj* a, Obj* b)
	{
		switch (a->type)
		{
			case Type::FLOAT:

				return a->_float > CastObj<j_float>(b);

			default: // CHAR, BOOL, INT

				return a->_int > CastObj<j_int>(b);
		}

		return false;
	}

	bool isGreaterEq(Obj* a, Obj* b)
	{
		switch (a->type)
		{
			case Type::FLOAT:

				return a->_float >= CastObj<j_float>(b);

			default: // CHAR, BOOL, INT

				return a->_int >= CastObj<j_int>(b);
		}

		return false;
	}
}