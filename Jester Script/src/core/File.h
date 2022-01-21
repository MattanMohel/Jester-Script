#pragma once

#include "Types.h"

namespace jts::file {

	// opens file with absolute path
	FILE* open(const str& path);
	// opens file relative to VM working path
	FILE* open(VM* vm, const str& path);

	// Takes a filepath and returns the file contents as string
	str readFile(VM* vm, FILE* file);

	// Takes a string source, converts it to VM code and executes
	void parseSrc(VM* vm, str src, bool run = true);

	str projectDir();
}

