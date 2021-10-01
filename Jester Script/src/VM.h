#ifndef VM_H
#define VM_H

#include "Types.h"

namespace jts
{
	// JTS virtual machine
	// --Stores state of JTS programs
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
		// Creates a new key-value symbol and emplaces it to the VM
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

		// Enter the JTS Read-Eval-Print-Loop (REPL)
		void BeginREPL(VM* vm);

		// Execute the current state of the VM
		Obj* RunVM(VM* vm);
	}
}

#endif

