#include "Operations.h"
#include "Execute.h"
#include "Object.h"

#include <iostream>

// char, int and bool share the same data -> for switch check if its float, otherwise operate on int --> transmutable for char bool & float

namespace jts
{
	template<> Obj* BinaryOpObj<BinaryOp::ADD>(Obj* a, Obj* b)
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

	template<> Obj* BinaryOpObj<BinaryOp::SUB>(Obj* a, Obj* b)
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
	
	template<> Obj* BinaryOpObj<BinaryOp::MUL>(Obj* a, Obj* b)
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
	
	template<> Obj* BinaryOpObj<BinaryOp::DIV>(Obj* a, Obj* b)
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

	template<> Obj* BinaryOpObj<BinaryOp::MOD>(Obj* a, Obj* b)
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

	template<> Obj* BinaryOpObj<BinaryOp::POW>(Obj* a, Obj* b)
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
	
	template<> Obj* BinaryOpObj<BinaryOp::LOG>(Obj* a, Obj* b)
	{
		// (log 2.0 4.0) = 2

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
	
	template<> Obj* BinaryOpObj<BinaryOp::SET>(Obj* a, Obj* b)
	{
		a->type = b->type;

		switch (b->type)
		{
			case Type::LIST:

				a->ret = b->ret;
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
	
	template<> Obj* BinaryOpObj<BinaryOp::QUOTE>(Obj* a, Obj* b)
	{
		ObjNode* cell = nullptr;
		ObjNode* args = nullptr;

		switch (b->spec)
		{
			case Spec::HEAD:

				args = b->_args;

				a->type = Type::LIST;
				a->spec = Spec::HEAD;

				a->ret = new Obj();

				a->_args = new ObjNode(new Obj { Type::QUOTE, Spec::VALUE });
				a->_args->value->_quote = args->value;

				cell = a->_args;

				while (args->next)
				{
					cell->next = new ObjNode(BinaryOpObj<BinaryOp::QUOTE>(args->next->value->ret, args->next->value));

					args = args->next;

					cell = cell->next;
				}

				return a; 

			default:

				Obj* quote = new Obj { Type::QUOTE, Spec::VALUE };
				quote->_quote = b;

				return quote;
		}
	}

	template<> Obj* UnaryOpObj<UnaryOp::INCR>(Obj* a)
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
	
	template<> Obj* UnaryOpObj<UnaryOp::DECR>(Obj* a)
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

	template<> Obj* UnaryOpObj<UnaryOp::SIN>(Obj* a)
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

	template<> Obj* UnaryOpObj<UnaryOp::ASIN>(Obj* a)
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

	template<> Obj* UnaryOpObj<UnaryOp::COS>(Obj* a)
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

	template<> Obj* UnaryOpObj<UnaryOp::ACOS>(Obj* a)
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

	template<> Obj* UnaryOpObj<UnaryOp::TAN>(Obj* a)
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

	template<> Obj* UnaryOpObj<UnaryOp::ATAN>(Obj* a)
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

	template<> Obj* UnaryOpObj<UnaryOp::LN>(Obj* a)
	{
		// (log 2.0 4.0) = 2

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

	template<> Obj* UnaryOpObj<UnaryOp::HASH>(Obj* a)
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

	Obj* SetState(Obj* a, bool state)
	{
		a->type = Type::BOOL;
		a->_bool = state;

		return a;
	}
}