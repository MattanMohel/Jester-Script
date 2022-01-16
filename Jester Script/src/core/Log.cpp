#include "Log.h"
#include "Object.h"
#include "Execute.h"
#include "Object.h"
#include "CppClass.h"
#include "CppFunc.h"
#include "JtsType.h"

#include <iostream>
#include <limits>

namespace jts {

	Obj* printObj(Obj* value, bool newLine) {
		std::cout << toString(value);

		if (newLine) std::cout << '\n';

		return value;
	}

	str toString(Obj* obj) {
		switch (obj->type) {
		case Type::NIL:

			return "nil";

		case Type::STRING:

			return *obj->_string;


		case Type::INT:

			return std::to_string(obj->_int);

		case Type::CHAR:

			return str(1, obj->_char);

		case Type::BOOL:

			return obj->_bool ? "true" : "false";

		case Type::QUOTE:
			
			switch (obj->_quote->type) {
			case Type::QUOTE:
			case Type::LIST:

				return toString(obj->_quote);

			default:

				return obj->_quote->symbol;
			}

		case Type::FLOAT:
		{
			str ret = std::to_string(obj->_float);
			ret.erase(ret.find_last_not_of('0') + 1, str::npos);

			return ret;
		}

		case Type::LIST:
		{
			str ret = "(";

			auto elem = obj->_args;

			if (elem) {
				while (elem && elem->nxt) {
					ret += toString(elem->val) + " ";
					elem = elem->nxt;
				}

				ret += toString(elem->val);
			}

			return ret + ')';
		}

		case Type::JTS_TYPE:
		{
			str ret = '(' + obj->symbol + " : ";

			bool first = true;

			for (auto& member : obj->_jtsType->members) {
				if (!first) ret += " ";

				ret += '(' + member.first + " : " + toString(member.second) + ')';

				first = false;
			}

			return ret + ')';
		}

		case Type::CPP_TYPE:
		{
			str ret = '(' + obj->symbol + " : ";

			bool first = true;

			for (auto& member : obj->_cppType->classTemplate->members) {
				if (!first) ret += " ";

				ret += '(' + member.first + " : " + toString(obj->_cppType->getMember_NonRef(member.first)) + ')';

				first = false;
			}

			return ret + ')';
		}

		default:

			return obj->symbol;
		}
	}
}