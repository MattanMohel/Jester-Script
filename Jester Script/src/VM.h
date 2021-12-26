#ifndef VM_H
#define VM_H

#include "Types.h"
#include "../utils/Pool.h"

#define DEBUG 1

namespace jts {

	// Jester Script virtual machine

	struct VM {
		ObjNode* stackPtrCur = nullptr;
		ObjNode* stackPtrBeg = nullptr;

		Tok* tokenPtrCur = nullptr;
		Tok* tokenPtrBeg = nullptr;

		std::unordered_map<str, Obj*> symbols;
		std::vector<SymbolMap*> scopes;
		SymbolMap* lexicalScope = nullptr;

		std::vector<void(*)(VM* vm)> libs;
	};

	namespace env {

		// Emplaces new object to VM with key

		void addSymbol(VM* vm, str key, Obj* value);

		// Gets object from VM by key

		Obj* getSymbol(VM* vm, str symbol);

		// Returns if a given symbol can shadow - if in global but not local scope

		bool canShadow(VM* vm, str symbol);

		// Asserts VM state

		inline void assert(bool cond, str mes, State warnType = State::ERR);

		// Executes VM

		Obj* run(VM* vm);

		// Executes VM in REPL mode

		void runREPL(VM* vm);

		/*
		  Takes a function pointer

		  The function should then emplace the necessary objects to the VM

		  (See examples in the Jts-Libs directory)
		*/
		void addLib(VM* vm, void(*lib)(VM* vm));

		/*
		  Takes JTS source code and parses/executes it to create a new object

		  Examples:

		  env::addSymbol(vm, "add", env::addSrc(vm, "[fn (a b) (+ a b)]"));
		   - the source evaluates to a function which is assigned to the symbol "add"
		   - symbol is then emplaced to the VM regularly
		*/
		Obj* addSrc(VM* vm, str src);

		/*
		  Takes a C++ function pointer which operates on Objects

		  (See examples in the Jts-Libs directory)
		*/
		Obj* addNative(Obj* (*native)(Obj*, ObjNode*, bool));

		// Adds an object symbol by T

		template<typename T>
		inline Obj* addConst(T value) {
			static_assert(false, "type unsupported");
		}

		template<> Obj* addConst(j_char  value);
		template<> Obj* addConst(j_bool  value);
		template<> Obj* addConst(j_int   value);
		template<> Obj* addConst(j_float value);
		template<> Obj* addConst(std::nullptr_t value);

		// Obj and ObjNode memory pools

		extern Pool<Obj>     glbl_objPool;
		extern Pool<ObjNode> glbl_nodePool;

		// Acquires an initialized ObjNode from Obj and ObjNode pool

		ObjNode* acquireNode();

		// Releases an ObjNode

		void releaseNode(ObjNode* node);
	}
}

#endif

