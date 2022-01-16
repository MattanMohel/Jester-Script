#ifndef LOG_H
#define LOG_H

#include "Types.h"

namespace jts
{
	// prints and returns an object
	Obj* printObj(Obj* value, bool newLine = false);

	// Takes object and returns its string value
	str toString(Obj* obj);

	template<typename... T>
	str mes(str message, T... args) { 
		std::vector<str> vec;
		(vec.emplace_back(args), ...);

		str temp = "";

		// "assigned value to % 
		for (const char& c : message) {
			if (c == '%') {
				temp += vec.front();
			}
			else {
				temp += c;
			}
		}

		return temp;
	}
}

#endif