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

		if (quote && obj->spec == Spec::SYMBOL)
		{			
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

			default:
			{
				std::cout << symbolOf(value);

				break;
			}
		}

		if (newLine) std::cout << '\n';

		return value;
	}
}