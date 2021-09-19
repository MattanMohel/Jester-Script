#pragma once

#include "Types.h"

namespace jts
{
	str ReadSrc(VM* vm, const str& path);
	void ParseSrc(VM* vm, str path);
}

