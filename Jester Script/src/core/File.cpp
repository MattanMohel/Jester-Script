#include "File.h"
#include "Lexer.h"
#include "Parser.h"
#include "VM.h"
#include "Log.h"

namespace jts {
	str readSrc(VM* vm, const str& path) {
		FILE* file = nullptr;

		fopen_s(&file, path.c_str(), "r");

		env::assert(!file, mes("file % doesn't exist", path));

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