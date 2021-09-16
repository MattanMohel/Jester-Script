#pragma once

#include <iostream>
#include <functional>
#include "File.h"
#include "VM.h"

#include "../Jts Libs/Lib.h"
#include "IterableStack.h"
using namespace jts;

int main(char** argc, int** argv)
{
	VM* vm = new VM();	

	env::AddLib(vm, StdLib);

	ParseFile(vm, "scripts/Jester.jts");
	env::RunVM(vm);
}
