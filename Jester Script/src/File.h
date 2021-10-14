#pragma once

#include "Types.h"

namespace jts
{
	str readSrc(VM* vm, const str& path);
	void parseSrc(VM* vm, str path);
}

