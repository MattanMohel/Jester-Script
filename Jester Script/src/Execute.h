#ifndef EXECUTE_H
#define EXECUTE_H

#define JTS_DEBUG 1

#include "Types.h"

namespace jts
{
	// Evaluates Obj, evaluates if func, returns self otherwise
	Obj* EvalObj(ObjNode* obj, bool eval = false);

	// Executes an Obj func
	Obj* ExecObj(Obj* ret, ObjNode* args, bool eval);

	// Returns Obj as integral value
	template<typename T>
	inline T CastObj(Obj* obj)
	{
		switch (obj->type)
		{
			case Type::CHAR:

				return static_cast<T>(obj->_char);

			case Type::BOOL:

				return static_cast<T>(obj->_bool);

			case Type::INT:

				return static_cast<T>(obj->_int);

			default: // case FLOAT

				return static_cast<T>(obj->_float);
		}
	}

	// Returns Type as JTS enum-Type
	template<typename T>
	inline Type GetType()
	{
		if (std::is_same<T, j_char>::value)
		{
			return Type::INT;
		}
		else if (std::is_same<T, j_bool>::value)
		{
			return Type::BOOL;
		}
		else if (std::is_same<T, j_int>::value)
		{
			return Type::INT;
		}
		else if (std::is_same<T, j_float>::value)
		{
			return Type::FLOAT;
		}
	}
}

#endif