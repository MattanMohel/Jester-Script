#pragma once

#include <iostream>
#include "File.h"
#include "VM.h"

#include "../Jts Libs/Lib.h"

using namespace jts;

int main(char** argc, int** argv)
{
	VM* vm = new VM();

	env::AddLib(vm, StdLib);

	ParseFile(vm, "scripts/Jester.jts");
	env::RunVM(vm);
}
