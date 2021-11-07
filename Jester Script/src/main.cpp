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

struct vec3
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

	env::addSymbol(vm, "Vec3", env::addClass<vec3>());
	env::addMember<vec3>(vm, "Vec3", "x", &vec3::x);
	env::addMember<vec3>(vm, "Vec3", "y", &vec3::y);
	env::addMember<vec3>(vm, "Vec3", "z", &vec3::z);
	env::addMethod<vec3>(vm, "Vec3", "mag", &vec3::mag);
	
	env_ADD_CLASS(vm, Timer);
	env_ADD_METHOD(vm, Timer, elapsed);
	env_ADD_METHOD(vm, Timer, reset);

	//parseSrc(vm, readSrc(vm, "scripts/Recursion.jts"));
	//env::run(vm);	
	parseSrc(vm, readSrc(vm, "scripts/Type.jts"));
	env::run(vm);	

	env::runREPL(vm);
}
