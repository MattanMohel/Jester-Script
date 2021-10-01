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
	VM* vm = new VM();	

	env::AddLib(vm, lib::StandardLib);
	env::AddLib(vm, lib::ArithmeticLib);
	env::AddLib(vm, lib::BooleanLib);

	env::AddSymbol(vm, "fact", env::AddBridge(fac));

	ParseSrc(vm, ReadSrc(vm, "scripts/Macro.jts"));
	env::RunVM(vm);

	env::BeginREPL(vm);
}
