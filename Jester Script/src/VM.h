#ifndef VM_H
#define VM_H

#include "Types.h"
#include "../utils/Pool.h"

#define CHECK_ERROR 1
#define DEBUG_ALLOC 0

#define START_OBJ_COUNT 250

namespace jts {

	// Jester Script virtual machine

	struct VM {

		std::unordered_map<str, Obj*> symbols;

		std::vector<void(*)(VM* vm)> libs;

		Pool<Node>* nodePool = nullptr;
		Pool<Obj>*  objPool  = nullptr;

		////////////////
		/////States/////
		////////////////

		Node* stackPtrCur = nullptr;
		Node* stackPtrBeg = nullptr;

		Tok* tokenPtrCur = nullptr;
		Tok* tokenPtrBeg = nullptr;

		bool eval = false; 
	};

	namespace env {

		//////////////////////
		/////Constructors/////
		//////////////////////

		VM* newVM();

		Node* newNode(VM* vm, Type t = Type::NIL, Spec s = Spec::SYMBOL);
		Node* newNode   (VM* vm, Obj* obj);
		void  releaseNode(VM* vm, Node* node);

		Obj* newObj(VM* vm, Type t = Type::NIL, Spec s = Spec::SYMBOL);
		Obj* newObj (VM* vm, Obj* obj);
		void releaseObj(VM* vm, Obj* obj);

		/////////////////
		/////Symbols/////
		/////////////////

		void addSymbol(VM* vm, const str& key, Obj* val);
		
		Obj* addNative     (Obj* (*native)(VM*, Node*));

		Obj* addSrcCode(VM* vm, const str& src);

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

		Obj* run(VM* vm);	
		void runREPL(VM* vm);

		Obj* beginScope(VM* vm, Node* params, std::function<Obj*(VM*)> body);

		void clear(VM* vm);

		///////////////////////
		/////Miscellaneous/////
		///////////////////////

		void addLib(VM* vm, void(*lib)(VM* vm));
		void addScript(VM* vm, const str& path);

		void assert(bool cond, const str& mes, State warnType = State::ERR);
	}
}

#endif

