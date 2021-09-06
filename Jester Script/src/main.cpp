#pragma once

#include <iostream>
#include "File.h"
#include "VM.h"
#include "Object.h"

using namespace jts;

void MathLib(VM* vm)
{
	env::AddNative(vm, "+", [](Obj* args)
	{
		args->_int = 0;
		Obj* rest = args->args;

		while (rest)
		{
			args->_int += rest->_int;
			rest = rest->args;
		}

		return rest;
	});	
	
	env::AddNative(vm, "-", [](Obj* args) -> Obj*
	{
		args->_int = 0;
		Obj* rest = args->args;

		while (rest)
		{
			args->_int -= rest->_int;
			rest = rest->args;
		}

		return rest;
	});	
	
	env::AddNative(vm, "set", [](Obj* args) -> Obj*
	{
		return nullptr;
	});	
	
	env::AddNative(vm, "print", [](Obj* args) -> Obj*
	{
		return nullptr;
	});
}

int main(char** argc, int** argv)
{
	VM* vm = new VM();

	env::AddLib(vm, MathLib);

	ParseFile(vm, "scripts/Jester.jts");
}
