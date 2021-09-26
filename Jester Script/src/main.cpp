#pragma once

#include <iostream>
#include <functional>
#include "File.h"
#include "VM.h"

#include "../Jts Libs/Standard.h"
#include "../Jts Libs/Arithmetic.h"
#include "../Jts Libs/Boolean.h"

using namespace jts;

int main(char** argc, int** argv)
{
	VM* vm = new VM();	

	env::AddLib(vm, lib::StandardLib);
	env::AddLib(vm, lib::ArithmeticLib);
	env::AddLib(vm, lib::BooleanLib);

	//ParseSrc(vm, ReadSrc(vm, "scripts/Testing.jts"));
	//env::RunVM(vm);

	env::BeginREPL(vm);
}
