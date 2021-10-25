#pragma once

#include "File.h"
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

int main(char** argc, int** argv)
{
	VM* vm = new VM();	

	env::addLib(vm, lib::StandardLib);
	env::addLib(vm, lib::ArithmeticLib);
	env::addLib(vm, lib::BooleanLib);

	env::addSymbol(vm, "reset", env::addBridge(reset));
	env::addSymbol(vm, "elapsed", env::addBridge(elapsed));

	parseSrc(vm, readSrc(vm, "scripts/Recursion.jts"));
	env::run(vm);
	parseSrc(vm, readSrc(vm, "scripts/Macros.jts"));
	env::run(vm);
	parseSrc(vm, readSrc(vm, "scripts/Games.jts"));
	env::run(vm);	
	parseSrc(vm, readSrc(vm, "scripts/Type.jts"));
	env::run(vm);	

	env::runREPL(vm);
}
