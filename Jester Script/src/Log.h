#ifndef LOG_H
#define LOG_H

#include "Types.h"

namespace jts
{
	// prints and returns an object

	Obj* printObj(Obj* value, bool newLine = false);


	// Takes object and returns its string value

	str toString(Obj* obj);
}

#endif