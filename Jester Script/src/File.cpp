#include "File.h"
#include "Lexer.h"
#include "Parser.h"
#include "VM.h"

namespace jts
{
	void ParseFile(VM* vm, str path)
	{
		FILE* file = nullptr;

		fopen_s(&file, path.c_str(), "r");

		TokenizeFile(vm, file);

		ParseTokens(vm);
	}
}