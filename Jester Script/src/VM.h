#ifndef VM_H
#define VM_H

#include "Types.h"
#include "Object.h"
#include "../utils/Pool.h"

namespace jts
{
	// Jester Script virtual machine

	struct VM
	{
		VM() : objectPool(25)
		{}		
		
		VM(size_t poolSize) : objectPool(poolSize)
		{}

		ObjNode* stackPtrCur = nullptr;
		ObjNode* stackPtrBeg = nullptr;

		Tok* tokenPtrCur = nullptr;
		Tok* tokenPtrBeg = nullptr;

		std::unordered_map<str, Obj*> symbols;

		std::vector<void(*)(VM* vm)> libs;

		Pool<Obj> objectPool;
	};

	namespace env
	{
		// Takes a key and value, emplaces to Vm as a key-value pair
		void AddSymbol(VM* vm, str key, Obj* value);

		Obj* AddNative(Obj* (*native)(Obj* ret, ObjNode* args, bool eval));

		template<typename T>
		inline Obj* AddConst(T value)
		{
			static_assert(false, "type unsupported");
		}

		template<> Obj* AddConst(j_char  value);
		template<> Obj* AddConst(j_bool  value);
		template<> Obj* AddConst(j_int   value);
		template<> Obj* AddConst(j_float value);
		template<> Obj* AddConst(std::nullptr_t value);

		void AddLib(VM* vm, void(*lib)(VM* vm));

		Obj* GetSymbol(VM* vm, str symbol);

		void RunREPL(VM* vm);

		Obj* Run(VM* vm);
	}
}

#endif

