#ifndef VM_H
#define VM_H

#include "Types.h"
#include "util/Pool.h"

#define CHECK_ERROR 1

#if CHECK_ERROR
#define ASSERT(cond, mes) if (cond) { printf("%s\n", mes); __debugbreak();}
#else
#define ASSERT(cond, mes) 
#endif

#define POOL_SZ 100

namespace jts {

	// Jester Script virtual machine

	struct VM {

		using Map = std::unordered_map<str, Obj*>;

		Map symbols;

		std::vector<void(*)(VM* vm)> libs;

		Pool_s<Node, POOL_SZ> nodePool;
		Pool_s<Obj,  POOL_SZ>  objPool;

		////////////////
		/////States/////
		////////////////

		Node* stackPtrCur = nullptr;
		Node* stackPtrBeg = nullptr;

		Tok* tokenPtrCur = nullptr;
		Tok* tokenPtrBeg = nullptr;

		bool eval = false; 

		str workingDir = "";
	};

	namespace env {

		//////////////////////
		/////Type Setters/////
		//////////////////////

		VM* newVM();

		Node* newNode (VM* vm, Obj* obj);

		void  releaseNode(VM* vm, Node* node);

		Obj* newObj(VM* vm, Type t = Type::NIL, Spec s = Spec::SYMBOL);

		Obj* newObj(VM* vm, Obj* obj);

		void releaseObj(VM* vm, Obj* obj);

		/////////////////
		/////Symbols/////
		/////////////////

		void addSymbol(VM* vm, const str& key, Obj* val);
		
		Obj* addNative(Obj* (*native)(VM*, Node*));

		Obj* addSrc   (VM* vm, const str& src);

		// Adds an object symbol of type
		template<typename T>
		inline Obj* addConst(T value) {
			static_assert(false, "type unsupported");
		}

		template<> Obj* addConst(jtsc val);
		template<> Obj* addConst(jtsb val);
		template<> Obj* addConst(jtsi val);
		template<> Obj* addConst(jtsf val);
		template<> Obj* addConst(std::nullptr_t value);

		Obj* getSymbol(VM* vm, const str& symbol);

		///////////////////
		/////Execution/////
		///////////////////

		Obj* run    (VM* vm);	

		Obj* runREPL(VM* vm);

		Node* bindEnv(VM* vm, Node* locals, Node* newVal);

		Node* bindEnv(VM* vm, Node* locPair);

		void  unbindEnv(VM* vm, Node* locals,  Node* prvVal);

		///////////////////
		/////Directory/////
		///////////////////

		const str& pwd(VM* vm);

		void cd(VM* vm, const str& path);

		str cat(VM* vm, const str& file);

		str ls(VM* vm);

		///////////////////////
		/////Miscellaneous/////
		///////////////////////

		void addLib(VM* vm, void(*lib)(VM* vm));

		void addScript(VM* vm, const str& path, bool abs = false, bool run = true);

		void assert(bool cond, const str& mes, State warnType = State::ERR);
	}
}

#endif

