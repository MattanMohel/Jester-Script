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
	Obj* evalObj(VM* vm, Obj* obj);

	/* 
		Executes amd returns an object with a callable type
		
		@param args : pointer to the first argument of the callable
		@param eval : wether or not to evaluate quote values
		@param top  : if true, all objects are released to pool upon termination of eval
	*/
	Obj* execObj(VM* vm, Node* args);

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

			default: // FLOAT

				return static_cast<T>(obj->_float);
		}
	}

	// Takes T and returns corresponding enum-Type value 

	template<typename T>
	inline Type getType()
	{
		if (std::is_same<T, jtsc>::value)
		{
			return Type::INT;
		}
		else if (std::is_same<T, jtsb>::value)
		{
			return Type::BOOL;
		}
		else if (std::is_same<T, jtsi>::value)
		{
			return Type::INT;
		}
		else if (std::is_same<T, jtsf>::value)
		{
			return Type::FLOAT;
		}
	}
}

#endif