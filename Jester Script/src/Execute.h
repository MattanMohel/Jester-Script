#ifndef EXECUTE_H
#define EXECUTE_H

#define JTS_DEBUG 1

#include "Types.h"
#include "Object.h"

namespace jts
{
	/* 
		Evaluates and returns an object

		@param eval : whether or not to evaluate quote values
		@param top  : if true, all objects are released to pool upon termination of eval
	*/
	Obj* evalObj(Obj* obj, bool eval = false, bool top = false);

	/* 
		Executes amd returns an object with a callable type
		
		@param args : pointer to the first argument of the callable
		@param eval : wether or not to evaluate quote values
		@param top  : if true, all objects are released to pool upon termination of eval
	*/
	Obj* execObj(ObjNode* args, bool eval, bool top = false);

	// Takes object and returns its T value

	template<typename T>
	inline T castObj(Obj* obj)
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

	// Takes T and returns corresponding enum-Type value 

	template<typename T>
	inline Type getType()
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