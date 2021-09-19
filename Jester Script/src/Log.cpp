#include "Log.h"
#include "Object.h"
#include "Execute.h"

#include <iostream>

namespace jts 
{
	Obj* PrintObj(ObjNode* in, bool newLine)
	{
		Obj* value = EvalObj(in);

		str upperStr = "";

		switch (value->type)
		{
			case Type::QUOTE:

				for (auto c : value->_quote->value->symbol)
				{
					upperStr += toupper(c);
				}

				std::cout<<upperStr; 

				break;

			case Type::CHAR:

				std::cout << value->_char;
				break;

			case Type::BOOL:

				std::cout << (value->_bool ? "true" : "false");
				break;

			case Type::INT:

				std::cout << value->_int;
				break;

			case Type::FLOAT:

				std::cout << value->_float;
				break;

			default: // case NIL
				std::cout << "nil";
				break;
		}

		if (newLine) std::cout << '\n';

		return value;
	}
}