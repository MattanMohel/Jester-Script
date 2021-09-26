#include "Log.h"
#include "Object.h"
#include "Execute.h"

#include <iostream>

namespace jts 
{
	Obj* PrintObj(Obj* value, bool newLine)
	{
		str upperStr = "";
		ObjNode* elem = nullptr;

		switch (value->type)
		{
			case Type::MACRO:
			case Type::JTS_FN:
			case Type::NATIVE:

				for (auto c : value->symbol)
				{
					upperStr += toupper(c);
				}

				std::cout << upperStr;

				break;

			case Type::QUOTE:

				if (value->spec == Spec::HEAD) break;
				
				for (auto c : value->_quote->symbol)
				{
					upperStr += toupper(c);
				}

				std::cout << upperStr;

				break;

			case Type::LIST:

				std::cout << "(";

				elem = value->_args;

				while (elem->next)
				{
					PrintObj(elem->value);
					std::cout << " ";

					elem = elem->next;
				}

				PrintObj(elem->value);

				std::cout << ")";

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