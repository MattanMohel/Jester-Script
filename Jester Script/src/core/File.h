#pragma once

#include "Types.h"

namespace jts
{
	// Takes a filepath and returns the file contents as string

	str readSrc(VM* vm, const str& path);

	// Takes a string source, converts it to VM code and executes

	void parseSrc(VM* vm, str src, bool run = true);
}

