#include "Log.h"
#include "Object.h"
#include "Execute.h"

#include <iostream>

namespace jts 
{
	Obj* PrintObj(Obj* value, bool newLine, bool head)
	{
		str upperStr = "";

		if (value->cell && head)
		{
			std::cout << '(';
		}

		switch (value->type)
		{
			case Type::QUOTE:

				for (auto c : value->_quote->value->symbol)
				{
					upperStr += toupper(c);
				}

				std::cout << upperStr; 

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

				if (value->fnType != FnType::NIL)
				{
					for (auto c : value->symbol)
					{
						upperStr += toupper(c);
					}

					std::cout << upperStr;
				}
				else
				{
					std::cout << "nil";
					break;
				}
		}

		if (value->cell)
		{
			std::cout<< ' ';

			PrintObj(value->cell->value, false, false);

			if (head) std::cout << ')';
		}

		if (newLine) std::cout << '\n';

		return value;
	}
}