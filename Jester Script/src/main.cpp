#pragma once

// NOTE: stop allocating initial parsing object directly, all allocations should be done through the memory pool
// MOVE: obj/node pool into vm

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

int main(char** argc, int** argv) {
	std::cout << "---------- Welcome to Jester-Script ----------\n\n";

	VM* vm = env::newVM();

	env::addLib(vm, lib::StandardLib);
	env::addLib(vm, lib::ArithmeticLib);
	env::addLib(vm, lib::BooleanLib);
	env::addLib(vm, lib::ListsLib);

	env::addScript(vm, "scripts/Jester.jts");

	env::runREPL(vm);
}
