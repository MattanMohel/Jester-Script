#ifndef VM_H
#define VM_H

#include "Types.h"

namespace jts
{
	struct VM
	{
		Obj* stackPtrCur = nullptr;
		Obj* stackPtrBeg = nullptr;

		Tok* tokenPtrCur = nullptr;
		Tok* tokenPtrBeg = nullptr;

		std::unordered_map<str, Obj*> symbols;

		std::unordered_map<str, Obj* (*)(Obj* params)> natives;

		std::vector<void(*)(VM* vm)> libs;
	};

	namespace env
	{
		void AddLib(VM* vm, void(*lib)(VM* vm));

		void AddNative(VM* vm, str value, Obj* (*native)(Obj* params));

		Obj* GetSymbol(VM* vm, Tok* tok);
	}
}

#endif

