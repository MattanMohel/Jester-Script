#include <filesystem>
#include <iostream>

#include "File.h"
#include "Lexer.h"
#include "Parser.h"
#include "VM.h"
#include "Log.h"

namespace jts::file {

	FILE* open(const str& path) {
		FILE* file = nullptr;

		fopen_s(&file, path.c_str(), "r");

		ASSERT(!file, "opened nil file " + path);

		return file;
	}	
	
	FILE* open(VM* vm, const str& path) {
		FILE* file = nullptr;

		str path_s = vm->workDir + '/' + path;

		fopen_s(&file, path_s.c_str(), "r");

		ASSERT(!file, "opened nil file " + path_s);

		return file;
	}

	str readFile(VM* vm, FILE* file) {
		fseek(file, 0, SEEK_END);
		long length = ftell(file);
		fseek(file, 0, SEEK_SET);

		str srcBuffer = "";
		srcBuffer.resize(length + 1);

		char ch = ' ';
		str::iterator bufferIt = srcBuffer.begin();

		while (ch != EOF) {
			ch = fgetc(file);
			*bufferIt = ch;

			++bufferIt;
		}

		return srcBuffer;
	}

	void parseSrc(VM* vm, str src, bool run) {
		tokenizeFile(vm, src);

		parseTokens(vm);

		if (run) env::run(vm);
	}

	str projectDir() {
		return std::filesystem::current_path().string();
	}
}