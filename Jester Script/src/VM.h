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
		template<typename T>
		inline Obj* AddConst(T value)
		{
			static_assert(false, "type unsupported");
		}
		 
		void AddLib(VM* vm, void(*lib)(VM* vm));
		void AddSymbol(VM* vm, str key, Obj* value);

		Obj* AddNative(Obj* (*native)(ObjNode* params));
		template<> Obj* AddConst(char  value);
		template<> Obj* AddConst(bool  value);
		template<> Obj* AddConst(int   value);
		template<> Obj* AddConst(float value);
		template<> Obj* AddConst(std::nullptr_t value);

		Obj* GetSymbol(VM* vm, Tok* tok);

		void RunVM(VM* vm);
	}
}

#endif

