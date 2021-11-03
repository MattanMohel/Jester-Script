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

struct Vec3
{
	float x = 1.0, y = 2.0, z = 3.0;

	float mag()
	{
		return sqrt(x*x + y*y + z*z);
	}
};

int main(char** argc, int** argv)
{
	VM* vm = new VM();

	env::addLib(vm, lib::StandardLib);
	env::addLib(vm, lib::ArithmeticLib);
	env::addLib(vm, lib::BooleanLib);
	env::addLib(vm, lib::ListsLib);

	env::addSymbol(vm, "Vec3", env::addClass<Vec3>());
	env::addMember<Vec3>(vm, "Vec3", "x", &Vec3::x);
	env::addMember<Vec3>(vm, "Vec3", "y", &Vec3::y);
	env::addMember<Vec3>(vm, "Vec3", "z", &Vec3::z); 
	env::addMethod<Vec3>(vm, "Vec3", "mag", &Vec3::mag);

	env::addSymbol(vm, "Timer", env::addClass<Timer>());
	env_ADD_METHOD(vm, Timer, elapsed);
	env_ADD_METHOD(vm, Timer, reset);

	parseSrc(vm, readSrc(vm, "scripts/Type.jts"));
	env::run(vm);

	env::runREPL(vm);
}
