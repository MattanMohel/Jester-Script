#include "Operations.h"
#include "Execute.h"
#include "Object.h"

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
	
	template<> Obj* BinaryOpObj<BinaryOp::SET>(ObjNode* a, ObjNode* b)
	{
		Obj* value = EvalObj(b);

		if (value->spec == Spec::VALUE)
		{
			a->value->spec = Spec::VALUE;
			a->value->type = value->type;

			switch (value->type)
			{
				case Type::FLOAT:

					a->value->_float = CastObj<float>(value);
					break;

				default: // CHAR, BOOL, INT

					a->value->_int = CastObj<int>(value);
					break;
			}
		}
		else
		{
			a->value->spec   =    Spec::CALL;
			a->value->fnType = value->fnType;

			switch (value->fnType)
			{
				case FnType::NATIVE:
					
					a->value->_native = value->_native;
					break;

				case FnType::JTS:
				
					a->value->_jtsFunc = value->_jtsFunc;
					break;
								
				default: // C_BRIDGE

					break;	
			}
		}

		return a->value;
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

		if (val1->fnType == FnType::NIL)
		{
			switch (val1->type)
			{
				case Type::FLOAT:

					return val1->_float == val2->_float;

				default: // CHAR, BOOL, INT

					return val1->_int == val2->_int;
			}
		}

		switch (val1->fnType)
		{
			case FnType::NATIVE:

				return val1->_native == val2->_native;

			default: // JTS

				return val1->_jtsFunc == val2->_jtsFunc;
		}

	}

	Obj* SetState(ObjNode* a, bool state)
	{
		a->value->type = Type::BOOL;
		a->value->_bool = state;

		return a->value;
	}
}