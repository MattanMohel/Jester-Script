#pragma once

#include <iostream>
#include <functional>
#include "File.h"
#include "VM.h"

#include "../Jts Libs/Standard.h"
#include "../Jts Libs/Arithmetic.h"
#include "../Jts Libs/Boolean.h"

using namespace jts;

// list x VS quote x

// (list x): list with value of evaluated x
// (quote x): list with value of literal x obj*

// (list + x x): list with value of +, num, num
// (quote (+ x x)): list with value of +, ltrl x, ltrl x 

int main(char** argc, int** argv)
{
	VM* vm = new VM();	

	env::AddLib(vm, lib::StandardLib);
	env::AddLib(vm, lib::ArithmeticLib);
	env::AddLib(vm, lib::BooleanLib);

	/*ParseSrc(vm, ReadSrc(vm, "scripts/Testing.jts"));
	env::RunVM(vm);*/

	env::BeginREPL(vm);
}
