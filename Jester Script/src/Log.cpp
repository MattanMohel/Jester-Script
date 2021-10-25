#include "Log.h"
#include "Object.h"
#include "Execute.h"

#include <iostream>
#include <limits>

namespace jts 
{
	str symbolOf(Obj* obj, bool quote)
	{
		str symbol = "";

		if (obj->spec == Spec::SYMBOL)
		{
			if (!quote) return toString(obj);
			
			for (auto c : obj->symbol)
			{
				symbol += toupper(c);
			}
		}
		else
		{
			symbol = toString(obj);
		}

		return symbol;
	}

	Obj* printObj(Obj* value, bool newLine)
	{
		switch (value->type)
		{
			case Type::NIL:
			case Type::CHAR:
			case Type::BOOL:
			case Type::INT:
			case Type::FLOAT:
			case Type::STRING:
			case Type::MAC_FN:
			case Type::JTS_FN:
			case Type::NAT_FN:
			case Type::CPP_FN:
			case Type::JTS_TYPE:
			{
				std::cout << symbolOf(value);

				break;
			}

			case Type::QUOTE:
			{
				if (value->type == Type::LIST) break;

				int quoteDepth = 0;
				Obj* quote = value->_quote;

				while (quote)
				{
					++quoteDepth;

					if (quote->type != Type::QUOTE)
					{
						std::cout << symbolOf(quote, quoteDepth > 0);
						break;
					}

					quote = quote->_quote;
				}

				break;
			}

			case Type::LIST:
			{
				std::cout << "(";

				auto* elem = value->_args;

				if (elem)
				{
					while (elem->next)
					{
						printObj(elem->value);
						std::cout << " ";

						elem = elem->next;
					}
					
					printObj(elem->value);
				}


				std::cout << ")";

				break;
			}
		}

		if (newLine) std::cout << '\n';

		return value;
	}
}