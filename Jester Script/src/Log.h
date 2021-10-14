#ifndef LOG_H
#define LOG_H

#include "Types.h"

namespace jts
{
	str symbolOf(Obj* obj, bool quote = false);
	Obj* printObj(Obj* value, bool newLine = false);
}

#endif