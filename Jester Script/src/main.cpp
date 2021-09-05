#pragma once

#include <iostream>
#include "File.h"
#include "VM.h"

using namespace jts;

int main(char** argc, int** argv)
{
	VM* vm = new VM();

	jts::ParseFile(vm, "scripts/Jester.jts");
}
