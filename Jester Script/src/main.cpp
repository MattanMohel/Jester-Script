#pragma once

#include "File.h"
#include "CppFunc.h"
#include "CppClass.h"
#include "VM.h"

#include "../Jts Libs/Standard.h"
#include "../Jts Libs/Arithmetic.h"
#include "../Jts Libs/Boolean.h"
#include "../Jts Libs/Lists.h"

#include "../utils/Timer.h"

using namespace jts;

int main(char** argc, int** argv)
{
	VM* vm = new VM();

	env::addLib(vm, lib::StandardLib);
	env::addLib(vm, lib::ArithmeticLib);
	env::addLib(vm, lib::BooleanLib);
	env::addLib(vm, lib::ListsLib);

	env_ADD_CLASS(vm, Timer);
	env_ADD_METHOD(vm, Timer, elapsed);
	env_ADD_METHOD(vm, Timer, reset);

	parseSrc(vm, readSrc(vm, "scripts/Recursion.jts"));
	parseSrc(vm, readSrc(vm, "scripts/Type.jts"));
	parseSrc(vm, readSrc(vm, "scripts/Function.jts"));

	env::runREPL(vm);
}
