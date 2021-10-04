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

	namespace env
	{
		extern Pool<Obj>     glbl_objPool;	
		extern Pool<ObjNode> glbl_nodePool;
		
		inline ObjNode* AcquireNode()
		{
			auto* node = glbl_nodePool.acquire();
			node->value = glbl_objPool.acquire();

			return node;
		}
				
		inline void ReleaseNode(ObjNode* node)
		{
			glbl_nodePool.release(node);
			glbl_objPool.release(node->value);
		}

		// Takes a key and value, emplaces to Vm as a key-value pair
		void AddSymbol(VM* vm, str key, Obj* value);

		Obj* AddNative(Obj* (*native)(Obj*, ObjNode*, bool));

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

