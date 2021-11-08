#ifndef LOG_H
#define LOG_H

#include "Types.h"

namespace jts
{
	// Returns the string symbol of an object

	str symbolOf(Obj* obj, bool quote = false);

	// prints and returns an object

	Obj* printObj(Obj* value, bool newLine = false);
}

#endif