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

namespace sample {
	Timer time;

	void reset() {
		time.reset();
	}

	float elapsed() {
		return time.elapsed();
	}
}

using namespace jts;

int main(char** argc, int** argv) {
	std::cout << "---------- Welcome to Jester-Script ----------\n\n";

	VM* vm = env::newVM();

	env::addLib(vm, lib::StandardLib);
	env::addLib(vm, lib::ArithmeticLib);
	env::addLib(vm, lib::BooleanLib);
	env::addLib(vm, lib::ListsLib);

	env::addSymbol(vm, "reset", env::addFunction(sample::reset));
	env::addSymbol(vm, "elapsed", env::addFunction(sample::elapsed));

	env::addScript(vm, "src/scripts/Jester.jts");

	env::runREPL(vm);
}
