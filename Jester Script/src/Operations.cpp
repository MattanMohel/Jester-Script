#include "Operations.h"
#include "Execute.h"
#include "Object.h"

namespace jts
{
	template<> Obj* BinaryOpObj<BinaryOp::ADD>(Obj* a, Obj* b)
	{
		switch (a->type)
		{
			case Type::CHAR:

				a->_char += CastObj<char>(EvalObj(b));
				break;

			case Type::BOOL:

				a->_bool += CastObj<bool>(EvalObj(b));
				break;

			case Type::INT:

				a->_int += CastObj<int>(EvalObj(b));
				break;

			default: // case FLOAT

				a->_float += CastObj<float>(EvalObj(b));
				break;
		}

		return a;
	}

	template<> Obj* BinaryOpObj<BinaryOp::SUB>(Obj* a, Obj* b)
	{
		switch (a->type)
		{
			case Type::CHAR:

				a->_char -= CastObj<char>(EvalObj(b));
				break;

			case Type::BOOL:

				a->_bool -= CastObj<bool>(EvalObj(b));
				break;

			case Type::INT:

				a->_int -= CastObj<int>(EvalObj(b));
				break;

			default: // case FLOAT

				a->_float -= CastObj<float>(EvalObj(b));
				break;
		}

		return a;
	}	
	
	template<> Obj* BinaryOpObj<BinaryOp::SET>(Obj* a, Obj* b)
	{
		switch (b->type)
		{
			case Type::CHAR:

				a->_char = EvalObj(b)->_char;
				a->type = Type::CHAR;
				break;

			case Type::BOOL:

				a->_bool = EvalObj(b)->_bool;
				a->type = Type::BOOL;
				break;

			case Type::INT:

				a->_int = EvalObj(b)->_int;
				a->type = Type::INT;
				break;

			default: // case FLOAT

				a->_float = EvalObj(b)->_float;
				a->type = Type::FLOAT;
				break;
		}

		return a;
	}
}