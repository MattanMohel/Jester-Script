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
		Obj* arg = args->rest;

		while (!arg->argsEnd)
		{
			args->_int += arg->_int;
			arg = arg->rest;
		}

		return arg;
	});	
	
	env::AddNative(vm, "-", [](Obj* args)
	{
		args->_int = 0;
		Obj* arg = args->rest;

		while (!arg->argsEnd)
		{
			args->_int -= arg->_int;
			arg = arg->rest;
		}

		return arg;
	});	
	
	env::AddNative(vm, "set", [](Obj* args)
	{
		// (set x 10) --> x becomes int
		// (+= x 10.0) --> x becomes float
		switch (args->rest->rest->type)
		{ 
			case Type::INT:
				//cast args->rest->_int to type
				args->rest->_int = args->rest->rest->_int;
				break;

			case Type::FLOAT:
				
				args->rest->_float = args->rest->rest->_float;
				break;			
			
			case Type::FLOAT:
				
				args->rest->_float = args->rest->rest->_float;
				break;
		}
	});	
	
	env::AddNative(vm, "print", [](Obj* args)
	{
		while (true)
		{

		}
	});
}

int main(char** argc, int** argv)
{
	VM* vm = new VM();

	env::AddLib(vm, MathLib);

	ParseFile(vm, "scripts/Jester.jts");
}
