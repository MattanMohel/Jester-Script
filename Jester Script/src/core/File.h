#pragma once

#include "Types.h"

namespace jts::file {

	// opens file with absolute path
	FILE* open(const str& path);

	// opens file with relative path
	FILE* open(VM* vm, const str& path);

	// returns the working directory
	str getWorkingDir();

	// returns vector of file-names in absolute path
	std::vector<str> getFiles(const str& path, bool rec = false);

	// returns vector of file-names in relative path
	std::vector<str> getFiles(VM* vm, const str& path, bool rec = false);

	// Takes a filepath and returns the file contents as string
	str readFile(FILE* file);

	// Takes a string source, converts it to VM code and executes
	void parseSrc(VM* vm, str src, bool run = true);
}

