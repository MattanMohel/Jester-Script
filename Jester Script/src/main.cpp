#pragma once

#include <iostream>
#include <functional>
#include "File.h"
#include "VM.h"

#include "../Jts Libs/Lib.h"
#include "../Jts Libs/Arithmetic.h"
#include "../Jts Libs/Boolean.h"
using namespace jts;

int main(char** argc, int** argv)
{
	VM* vm = new VM();	

	env::AddLib(vm, lib::StandardLib);
	env::AddLib(vm, lib::ArithmeticLib);
	env::AddLib(vm, lib::BooleanLib);

	ParseFile(vm, "scripts/Jester.jts");
	env::RunVM(vm);
}
