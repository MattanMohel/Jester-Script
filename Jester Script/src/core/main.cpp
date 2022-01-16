#pragma once

// NOTE: stop allocating initial parsing object directly, all allocations should be done through the memory pool
// MOVE: obj/node pool into vm

#include "File.h"
#include "CppFunc.h"
#include "CppClass.h"
#include "VM.h"

#include "prelude/Standard.h"
#include "prelude/Arithmetic.h"
#include "prelude/Boolean.h"
#include "prelude/Lists.h"

#include "util/Timer.h"

using namespace jts;

int main(char** argc, int** argv) {
	std::cout << "---------- Welcome to Jester-Script ----------\n\n";

	VM* vm = env::newVM();

	env::addLib(vm, lib::StandardLib);
	env::addLib(vm, lib::ArithmeticLib);
	env::addLib(vm, lib::BooleanLib);
	env::addLib(vm, lib::ListsLib);

	env::addScript(vm, "src/scripts/Jester.jts");

	env::runREPL(vm);
}
