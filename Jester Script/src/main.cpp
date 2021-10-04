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

#include "../utils/Timer.h"

//ISSUES
//1) cppBrdige assigns parameters in backwards order
//2) in jtsFunc in recursive calls if you modify parameter twice modification carries through (see "test" in Macro.jts

using namespace jts;

Timer timer;

void reset()
{
	timer.reset();
}

float elapsed()
{
	return timer.elapsed();
}

int fib(int n)
{
	if (n <= 1)
		return n;

	return fib(n - 1) + fib(n - 2);
}

int main(char** argc, int** argv)
{
	VM* vm = new VM();	

	env::AddLib(vm, lib::StandardLib);
	env::AddLib(vm, lib::ArithmeticLib);
	env::AddLib(vm, lib::BooleanLib);

	env::AddSymbol(vm, "reset", env::AddBridge(reset));
	env::AddSymbol(vm, "elapsed", env::AddBridge(elapsed));
	env::AddSymbol(vm, "c-fib", env::AddBridge(fib));

	ParseSrc(vm, ReadSrc(vm, "scripts/Macro.jts"));
	env::Run(vm);

	env::RunREPL(vm);
}
