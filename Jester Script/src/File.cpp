#include "File.h"
#include "Lexer.h"
#include "VM.h"

namespace jts
{
	void ParseFile(VM* vm, str path)
	{
		FILE* file = nullptr;

		fopen_s(&file, path.c_str(), "r");

		vm->Assert(file == nullptr, "could not open file " + path);

		TokenizeFile(vm, file);
	}
}