#ifndef EXECUTE_H
#define EXECUTE_H

#define JTS_DEBUG 1

#include "Types.h"
#include "Object.h"

namespace jts
{
	/* 
		Evaluates an object and returns its evaluated value

		@param obj  : the object to be evaluated
		@param eval : whether or not to evaluate quote values
	*/
	Obj* EvalObj(Obj* obj, bool eval = false);

	/* 
		Executes an object with a callable type and returns its executed value
		
		@param args - pointer to the first argument of the callable
		@param eval - wether or not to evaluate quote values
	*/
	Obj* ExecObj(ObjNode* args, bool eval);

	/*
		Takes object and returns as string
	*/
	str ToString(Obj* obj);

	/* 
		Takes an object and returns the value of the object casted into type T 
	*/
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

	/* 
		Takes type T and returns the corresponding enum Type value 
	*/
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