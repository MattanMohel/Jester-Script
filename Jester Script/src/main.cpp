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

	VM* vm = new VM();

	env::addLib(vm, lib::StandardLib);
	env::addLib(vm, lib::ArithmeticLib);
	env::addLib(vm, lib::BooleanLib);
	env::addLib(vm, lib::ListsLib);

	env::addSrc(vm, "(defn fac [n] (if (= n 0) 1 (* n (fac (- n 1)))))");
	env::addSrc(vm, "(defn when [cond result] (if cond result nil))");
	env::addSrc(vm, "(defn impl-or [cond-lst] (set stop F) (iterate cond cond-lst (if (not stop) (set stop ~cond) nil)) stop)");

	env::runREPL(vm);
}
