#pragma once

#include <iostream>
#include <functional>
#include "File.h"
#include "Operations.h"
#include "cppFunc.h"
#include "VM.h"

#include "../Jts Libs/Standard.h"
#include "../Jts Libs/Arithmetic.h"
#include "../Jts Libs/Boolean.h"

using namespace jts;

int fac(int num)
{
	if (num == 1) return 1;
	return num * fac(num - 1);
}

int main(char** argc, int** argv)
{
	VM* vm = new VM(3);	

	env::AddLib(vm, lib::StandardLib);
	env::AddLib(vm, lib::ArithmeticLib);
	env::AddLib(vm, lib::BooleanLib);

	ParseSrc(vm, ReadSrc(vm, "scripts/Macro.jts"));
	env::Run(vm);

	env::RunREPL(vm);
}
