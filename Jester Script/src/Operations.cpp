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
			case Type::CHAR:

				a->value->_char += CastObj<char>(EvalObj(b));
				break;

			case Type::BOOL:

				a->value->_bool += CastObj<bool>(EvalObj(b));
				break;

			case Type::INT:

				a->value->_int += CastObj<int>(EvalObj(b));
				break;

			case Type::FLOAT:

				a->value->_float += CastObj<float>(EvalObj(b));
				break;
		}

		return a->value;
	}

	template<> Obj* BinaryOpObj<BinaryOp::SUB>(ObjNode* a, ObjNode* b)
	{
		switch (a->value->type)
		{
			case Type::CHAR:

				a->value->_char -= CastObj<char>(EvalObj(b));
				break;

			case Type::BOOL:

				a->value->_bool -= CastObj<bool>(EvalObj(b));
				break;

			case Type::INT:

				a->value->_int -= CastObj<int>(EvalObj(b));
				break;

			case Type::FLOAT:

				a->value->_float -= CastObj<float>(EvalObj(b));
				break;
		}

		return a->value;
	}		
	
	template<> Obj* BinaryOpObj<BinaryOp::MUL>(ObjNode* a, ObjNode* b)
	{
		switch (a->value->type)
		{
			case Type::CHAR:

				a->value->_char *= CastObj<char>(EvalObj(b));
				break;

			case Type::BOOL:

				a->value->_bool *= CastObj<bool>(EvalObj(b));
				break;

			case Type::INT:

				a->value->_int *= CastObj<int>(EvalObj(b));
				break;

			case Type::FLOAT:

				a->value->_float *= CastObj<float>(EvalObj(b));
				break;
		}

		return a->value;
	}		
	
	template<> Obj* BinaryOpObj<BinaryOp::DIV>(ObjNode* a, ObjNode* b)
	{
		switch (a->value->type)
		{
			case Type::CHAR:

				a->value->_char /= CastObj<char>(EvalObj(b));
				break;

			case Type::BOOL:

				a->value->_bool /= CastObj<bool>(EvalObj(b));
				break;

			case Type::INT:

				a->value->_int /= CastObj<int>(EvalObj(b));
				break;

			case Type::FLOAT:

				a->value->_float /= CastObj<float>(EvalObj(b));
				break;
		}

		return a->value;
	}	
	
	template<> Obj* BinaryOpObj<BinaryOp::SET>(ObjNode* a, ObjNode* b)
	{
		Obj* value = EvalObj(b);

		switch (value->type)
		{
			case Type::CHAR:

				a->value->_char = value->_char;
				a->value->type = Type::CHAR;
				break;

			case Type::BOOL:

				a->value->_bool = value->_bool;
				a->value->type = Type::BOOL;
				break;

			case Type::INT:

				a->value->_int = value->_int;
				a->value->type = Type::INT;
				break;

			case Type::FLOAT:

				a->value->_float = value->_float;
				a->value->type = Type::FLOAT; 
				break;

			case Type::JTS_FUNC:

				a->value->_jtsFunc = value->_jtsFunc;
				a->value->fnType = FnType::JTS_FUNC;
				a->value->type = Type::JTS_FUNC;
				a->value->spec = Spec::FUNC;
				break;
		}

		return a->value;
	}

	template<> Obj* UnaryOpObj<UnaryOp::INCR>(ObjNode* a)
	{
		switch (a->value->type)
		{
			case Type::CHAR:

				++a->value->_char;
				break;

			case Type::BOOL:

				++a->value->_int;
				break;

			case Type::INT:

				++a->value->_int;
				break;

			case Type::FLOAT:

				++a->value->_float;
				break;
		}

		return a->value;
	}	
	
	template<> Obj* UnaryOpObj<UnaryOp::DECR>(ObjNode* a)
	{
		switch (a->value->type)
		{
			case Type::CHAR:

				--a->value->_char;
				break;

			case Type::BOOL:

				--a->value->_int; // Cant decrement bool
				break;

			case Type::INT:

				--a->value->_int;
				break;

			case Type::FLOAT:

				--a->value->_float;
				break;
		}

		return a->value;
	}
}