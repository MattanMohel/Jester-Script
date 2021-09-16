#ifndef VM_H
#define VM_H

#include "Types.h"

namespace jts
{
	struct VM
	{
		ObjNode* stackPtrCur = nullptr;
		ObjNode* stackPtrBeg = nullptr;

		Tok* tokenPtrCur = nullptr;
		Tok* tokenPtrBeg = nullptr;

		std::unordered_map<str, Obj*> symbols;

		std::vector<void(*)(VM* vm)> libs;
	};

	namespace env
	{
		void AddLib(VM* vm, void(*lib)(VM* vm));

		void AddNative(VM* vm, str value, Obj* (*native)(ObjNode* params));

		Obj* GetSymbol(VM* vm, Tok* tok);

		void RunVM(VM* vm);
	}
}

#endif

