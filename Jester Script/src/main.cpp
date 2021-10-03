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

//ISSUES
//1) cppBrdige assigns parameters in backwards order
//2) in jtsFunc in recursive calls if you modify parameter twice modification carries through (see "test" in Macro.jts

using namespace jts;

int cppFac(int num)
{
	if (num == 1) return 1;
	return num * cppFac(num - 1);
}

int cppTest(int x, int y)
{
	std::cout << "X" << x << std::endl << "Y" << y << std::endl;

	if (x == 0) return y;
	return cppTest(x - 1, x + y);
}

int main(char** argc, int** argv)
{
	VM* vm = new VM();	

	env::AddLib(vm, lib::StandardLib);
	env::AddLib(vm, lib::ArithmeticLib);
	env::AddLib(vm, lib::BooleanLib);

	env::AddSymbol(vm, "cppFac", env::AddBridge(cppFac));
	env::AddSymbol(vm, "cppTest", env::AddBridge(cppTest));

	ParseSrc(vm, ReadSrc(vm, "scripts/Macro.jts"));
	env::Run(vm);

	env::RunREPL(vm);
}
