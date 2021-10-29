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
		ObjNode* stackPtrCur = nullptr;
		ObjNode* stackPtrBeg = nullptr;

		Tok* tokenPtrCur = nullptr;
		Tok* tokenPtrBeg = nullptr;

		std::unordered_map<str, Obj*> symbols;

		std::vector<void(*)(VM* vm)> libs;
	};

	struct SymbolMap
	{
		std::unordered_map<str, Obj*> symbols;
	};

	namespace env
	{
		extern Pool<Obj>     glbl_objPool;	
		extern Pool<ObjNode> glbl_nodePool;
		
		ObjNode* acquireNode();
				
		void releaseNode(ObjNode* node);

		void assert(VM* vm, bool cond, str mes, State warnType = State::ERR);

		// Takes a key and value, emplaces to Vm as a key-value pair
		void addSymbol(VM* vm, str key, Obj* value);

		void addSrc(VM* vm, str src);

		Obj* addNative(Obj* (*native)(Obj*, ObjNode*, bool));

		template<typename T>
		inline Obj* addConst(T value)
		{
			static_assert(false, "type unsupported");
		}

		template<> Obj* addConst(j_char  value);
		template<> Obj* addConst(j_bool  value);
		template<> Obj* addConst(j_int   value);
		template<> Obj* addConst(j_float value);
		template<> Obj* addConst(std::nullptr_t value); 


		void addLib(VM* vm, void(*lib)(VM* vm));

		Obj* getSymbol(VM* vm, str symbol);

		void runREPL(VM* vm);

		Obj* run(VM* vm);
	}
}

#endif

