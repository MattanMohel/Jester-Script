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

		str path_s = vm->workingDir + '/' + path;

		fopen_s(&file, path_s.c_str(), "r");

		ASSERT(!file, "opened nil file " + path_s);

		return file;
	}

	str getWorkingDir() {
		return std::filesystem::current_path().string();
	}

	std::vector<str> getFiles(const str& path, bool rec) {
		std::vector<str> files;

		for (auto const& entry : std::filesystem::directory_iterator{ path }) {
			files.emplace_back(entry.path().string());
		}

		return files;
	}	
	
	std::vector<str> getFiles(VM* vm, const str& path, bool rec) {
		std::vector<str> files;

		for (auto const& entry : std::filesystem::directory_iterator{ env::pwd(vm) + path }) {
			str file = entry.path().string();
			files.emplace_back(file.substr(file.find_last_of('\\') + 1));
		}

		return files;
	}

	str readFile(FILE* file) {
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

}