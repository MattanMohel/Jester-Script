#include "Operations.h"
#include "Execute.h"
#include "Object.h"

#include <iostream>

// char, int and bool share the same data -> for switch check if its float, otherwise operate on int --> transmutable for char bool & float

namespace jts
{
	template<> Obj* BinaryOpObj<BinaryOp::ADD>(ObjNode* a, ObjNode* b)
	{
		switch (a->value->type)
		{
			case Type::FLOAT:

				a->value->_float += CastObj<float>(EvalObj(b));
				break;
			
			default: // CHAR, BOOL, INT
				
				a->value->_int += CastObj<int>(EvalObj(b));
				break;
		}

		return a->value;
	}

	template<> Obj* BinaryOpObj<BinaryOp::SUB>(ObjNode* a, ObjNode* b)
	{
		switch (a->value->type)
		{
			case Type::FLOAT:

				a->value->_float -= CastObj<float>(EvalObj(b));
				break;

			default: // CHAR, BOOL, INT
				
				a->value->_int -= CastObj<int>(EvalObj(b));
				break;
		}

		return a->value;
	}		
	
	template<> Obj* BinaryOpObj<BinaryOp::MUL>(ObjNode* a, ObjNode* b)
	{
		switch (a->value->type)
		{
			case Type::FLOAT:

				a->value->_float *= CastObj<float>(EvalObj(b));
				break;

			default: // CHAR, BOOL, INT
				
				a->value->_int *= CastObj<int>(EvalObj(b));
				break;
		}

		return a->value;
	}		
	
	template<> Obj* BinaryOpObj<BinaryOp::DIV>(ObjNode* a, ObjNode* b)
	{
		switch (a->value->type)
		{
			case Type::FLOAT:

				a->value->_float /= CastObj<float>(EvalObj(b));
				break;

			default: // CHAR, BOOL, INT
				
				a->value->_int /= CastObj<int>(EvalObj(b));
				break;
		}

		return a->value;
	}	

	template<> Obj* BinaryOpObj<BinaryOp::MOD>(ObjNode* a, ObjNode* b)
	{
		switch (a->value->type)
		{
			case Type::FLOAT:

				a->value->_float = (int)a->value->_float % CastObj<int>(EvalObj(b));
				break;

			default: // CHAR, BOOL, INT
				
				a->value->_int %= CastObj<int>(EvalObj(b));
				break;
		}

		return a->value;
	}

	template<> Obj* BinaryOpObj<BinaryOp::POW>(ObjNode* a, ObjNode* b)
	{
		switch (a->value->type)
		{
			case Type::FLOAT:

				a->value->_float = pow(a->value->_float, CastObj<int>(EvalObj(b)));
				break;

			default: // CHAR, BOOL, INT

				a->value->_int = pow(a->value->_int, CastObj<int>(EvalObj(b)));
				break;
		}

		return a->value;
	}
	
	template<> Obj* BinaryOpObj<BinaryOp::SET>(ObjNode* a, ObjNode* b)
	{
		Obj* value2 = EvalObj(b);

		Obj* value1 = (a->value->spec == Spec::CALL_BEG)? a->value : EvalObj(a);

		value1->type = value2->type;

		switch (value2->type)
		{
			case Type::LIST:

				value1->ret   = value2->ret;
				value1->spec  = value2->spec;
				value1->_args = value2->_args;

				break;

			case Type::NATIVE:

				value1->_native = value2->_native;
				break;

			case Type::JTS:
					
				value1->_jtsFunc = value2->_jtsFunc;
				break;

			case Type::C_BRIDGE:
				// assign
				break;

			case Type::FLOAT:

				value1->_float = CastObj<float>(value2);
				break;

			default: // CHAR, BOOL, INT

				value1->_int = CastObj<int>(value2);
				break;
		}

		return value1;
	}

	template<> Obj* UnaryOpObj<UnaryOp::INCR>(ObjNode* a)
	{
		switch (a->value->type)
		{
			case Type::FLOAT:

				++a->value->_float;
				break;

			default: // CHAR, BOOL, INT
				++a->value->_int;
				break;
		}

		return a->value;
	}	
	
	template<> Obj* UnaryOpObj<UnaryOp::DECR>(ObjNode* a)
	{
		switch (a->value->type)
		{
			case Type::FLOAT:

				--a->value->_float;
				break;

			default: // CHAR, BOOL, INT
				--a->value->_int;
				break;
		}

		return a->value;
	}

	template<> Obj* UnaryOpObj<UnaryOp::SIN>(ObjNode* a)
	{
		switch (a->value->type)
		{
			case Type::FLOAT:

				a->value->_float = sin(a->value->_float);
				break;

			default: // CHAR, BOOL, INT
				
				a->value->_int = sin(a->value->_int);
				break;
		}

		return a->value;
	}

	template<> Obj* UnaryOpObj<UnaryOp::ASIN>(ObjNode* a)
	{
		switch (a->value->type)
		{
			case Type::FLOAT:

				a->value->_float = asin(a->value->_float);
				break;

			default: // CHAR, BOOL, INT

				a->value->_int = asin(a->value->_int); 
				break;
		}

		return a->value;
	}

	template<> Obj* UnaryOpObj<UnaryOp::COS>(ObjNode* a)
	{
		switch (a->value->type)
		{
			case Type::FLOAT:

				a->value->_float = cos(a->value->_float);
				break;

			default: // CHAR, BOOL, INT

				a->value->_int = cos(a->value->_int);
				break;
		}

		return a->value;
	}

	template<> Obj* UnaryOpObj<UnaryOp::ACOS>(ObjNode* a)
	{
		switch (a->value->type)
		{
			case Type::FLOAT:

				a->value->_float = acos(a->value->_float);
				break;

			default: // CHAR, BOOL, INT

				a->value->_int = acos(a->value->_int);
				break;
		}

		return a->value;
	}

	template<> Obj* UnaryOpObj<UnaryOp::TAN>(ObjNode* a)
	{
		switch (a->value->type)
		{
			case Type::FLOAT:

				a->value->_float = tan(a->value->_float);
				break;

			default: // CHAR, BOOL, INT

				a->value->_int = tan(a->value->_int);
				break;
		}

		return a->value;
	}

	template<> Obj* UnaryOpObj<UnaryOp::ATAN>(ObjNode* a)
	{
		switch (a->value->type)
		{
			case Type::FLOAT:

				a->value->_float = atan(a->value->_float);
				break;

			default: // CHAR, BOOL, INT

				a->value->_int = atan(a->value->_int);
				break;
		}

		return a->value;
	}

	bool isTrue(ObjNode* a)
	{
		Obj* value = EvalObj(a);

		switch (value->type)
		{
			case Type::FLOAT:

				return (bool)value->_float;
				break;

			default: // CHAR, BOOL, INT

				return (bool)value->_int;
				break;
		}
	}

	bool isEqual(ObjNode* a, ObjNode* b)
	{
		Obj* val1 = EvalObj(a);
		Obj* val2 = EvalObj(b);

		switch (val1->type)
		{
			case Type::FLOAT:

				return val1->_float == val2->_float;

			default: // CHAR, BOOL, INT

				return val1->_int == val2->_int;
		}
	}

	bool isGreater(ObjNode* a, ObjNode* b)
	{
		Obj* val1 = EvalObj(a);
		Obj* val2 = EvalObj(b);

		switch (val1->type)
		{
			case Type::FLOAT:

				return val1->_float > CastObj<float>(val2);

			default: // CHAR, BOOL, INT

				return val1->_int > CastObj<int>(val2);
		}

		return false;
	}

	bool isGreaterEq(ObjNode* a, ObjNode* b)
	{
		Obj* val1 = EvalObj(a);
		Obj* val2 = EvalObj(b);

		switch (val1->type)
		{
			case Type::FLOAT:

				return val1->_float >= CastObj<float>(val2);

			default: // CHAR, BOOL, INT

				return val1->_int >= CastObj<int>(val2);
		}

		return false;
	}

	Obj* SetState(ObjNode* a, bool state)
	{
		a->value->type = Type::BOOL;
		a->value->_bool = state;

		return a->value;
	}
}