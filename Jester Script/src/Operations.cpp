#include "Operations.h"
#include "Execute.h"
#include "Object.h"
#include "VM.h"
#include <iostream>

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
		a->type = b->type;

		switch (b->type)
		{
			case Type::LIST:

				a->spec = b->spec;
				a->_args = b->_args;

				break;

			case Type::NATIVE:

				a->_native = b->_native;
				break;

			case Type::JTS_FN:

				a->_jtsFunc = b->_jtsFunc;
				break;

			case Type::CPP_FN:
				// assign
				break;

			case Type::QUOTE:

				a->_quote = b->_quote;
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
		//ObjNode* cell = nullptr;
		//ObjNode* args = nullptr;

		//Obj* quote = env::glbl_objectPool.acquire();

		//switch (a->type)
		//{
		//	case Type::LIST:

		//		args = b->_args;

		//		quote->type = Type::LIST;

		//		a->_args = env::glbl_nodePool.acquire(); //new Obj { Type::QUOTE, Spec::VALUE });
		//		a->_args->value = env::glbl_objectPool.acquire();
		//		*a->_args->value = Obj { Type::QUOTE, Spec::VALUE };

		//		a->_args->value->_quote = args->value;

		//		cell = a->_args;

		//		while (args->next)
		//		{
		//			cell->next = new ObjNode(BinaryOp<Binary::QUOTE>(args->next->value->ret, args->next->value));

		//			args = args->next;

		//			cell = cell->next;
		//		}

		//		return a; 

		//	default:

		//		Obj* quote = new Obj { Type::QUOTE, Spec::VALUE };
		//		quote->_quote = b;

		//		return quote;
		//}

		return nullptr;
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

				return (j_bool)a->_int;
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