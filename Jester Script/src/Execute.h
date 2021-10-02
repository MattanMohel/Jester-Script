#ifndef EXECUTE_H
#define EXECUTE_H

#define JTS_DEBUG 1

#include "Types.h"

namespace jts
{
	/* 
		Evaluates an object and returns its evaluated value

		- ObjNode* obj : the object to be evaluated
		- bool eval : wether or not to evaluate quote values
	*/
	Obj* EvalObj(ObjNode* obj, bool eval = false);

	/* 
		Executes an object with a callable type and returns its executed value
		
		- Obj* ret : the return Obj* of the callable object
		- ObjNode* args : pointer to the first argument of the callable
		- bool eval : wether or not to evaluate quote values
	*/
	Obj* ExecObj(Obj* ret, ObjNode* args, bool eval);

	/* Takes an object and returns the value of the object casted into type T */
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

	/* Takes type T and returns the corresponding enum Type value */
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