#include "VM.h"
#include "Token.h"
#include "Execute.h"
#include "Operations.h"
#include "Object.h"
#include "Lexer.h"
#include "File.h"
#include "Log.h"

#include <iostream>

namespace jts {
	namespace env {

		//////////////////////
		/////Constructors/////
		//////////////////////

		VM* newVM() {
			VM* vm = new VM();

			vm->objPool = new Pool<Obj>("Object", START_OBJ_COUNT, [](Obj* val) {
				val->refCount = nullptr;
				val->spec = Spec::SYMBOL;
				val->type = Type::NIL;
				val->_int = 0;

				return val;
			});

			vm->nodePool = new Pool<Node>("Node", START_OBJ_COUNT, [](Node* val) {
				val->nxt = nullptr;
				val->val = nullptr;

				return val;
			});

			return vm;
		}

		Node* newNode(VM* vm, Type t, Spec s) {
			auto* node = vm->nodePool->acquire();

			node->val = env::newObj(vm, t, s);

			return node;
		}
				
		Node* newNode(VM* vm, Obj* obj) {
			auto* node = vm->nodePool->acquire();

			node->val = obj;

			return node;
		}

		void releaseNode(VM* vm, Node* node) {
			vm->nodePool->release(node);
		}

		Obj* newObj(VM* vm, Type t, Spec s) {
			auto obj = vm->objPool->acquire();
			obj->type = t;
			obj->spec = s;

			return obj;
		}

		Obj* newObj(VM* vm, Obj* val) {
			return setObj(vm, vm->objPool->acquire(), val);
		}

		void releaseObj(VM* vm, Obj* obj) {
			vm->objPool->release(obj);
		}

		/////////////////
		/////Symbols/////
		/////////////////

		void addSymbol(VM* vm, const str& symbol, Obj* val) {
			bool hasKey = env::getSymbol(vm, symbol);

			assert(false, "creating duplicate symbol " + symbol);

			vm->symbols.emplace(symbol, val);
		}

		Obj* addNative(Obj* (*native)(VM*, Node*)) {
			Obj* obj = new Obj();

			obj->_native = native;
			obj->type = Type::NAT_FN;
			obj->spec = Spec::SYMBOL;

			obj->constant = true;

			return obj;
		}		

		Obj* addSrcCode(VM* vm, str src) {
			src += EOF;
			parseSrc(vm, src);

			return env::run(vm);
		}

		template<> Obj* addConst(jtsc val) {
			Obj* obj = new Obj{ Type::CHAR, Spec::SYMBOL };
			obj->constant = true;
			obj->_char = val;

			obj->constant = true;

			return obj;
		}

		template<> Obj* addConst(jtsb val) {
			Obj* obj = new Obj{ Type::BOOL, Spec::SYMBOL };
			obj->constant = true;
			obj->_bool = val;

			obj->constant = true;

			return obj;
		}

		template<> Obj* addConst(jtsi  val) {
			Obj* obj = new Obj{ Type::INT, Spec::SYMBOL };
			obj->constant = true;
			obj->_int = val;

			obj->constant = true;

			return obj;
		}

		template<> Obj* addConst(jtsf val) {
			Obj* obj = new Obj{ Type::FLOAT, Spec::SYMBOL };
			obj->constant = true;
			obj->_float = val;

			obj->constant = true;

			return obj;
		}

		template<> Obj* addConst(std::nullptr_t val) {
			Obj* obj = new Obj{ Type::NIL, Spec::SYMBOL };

			obj->constant = true;

			return obj;
		}

		Obj* getSymbol(VM* vm, const str& symbol) {
			if (vm->symbols.find(symbol) != vm->symbols.end()) {
				return vm->symbols[symbol]; 
			}

			return nullptr;
		}

		///////////////////
		/////Execution/////
		///////////////////

		Obj* run(VM* vm) {


			vm->stackPtrCur = vm->stackPtrBeg;

			while (vm->stackPtrCur->nxt) {
				evalObj(vm, vm->stackPtrCur->val);

				vm->stackPtrCur = vm->stackPtrCur->nxt;
			}

			return evalObj(vm, vm->stackPtrCur->val);
		}

		void runREPL(VM* vm) {
			str src;

			size_t inputCount = 0;

			while (1) {
				std::cout << "[" << inputCount++ << "]>> ";
				std::getline(std::cin, src);
				src += EOF;

				if (src.empty()) continue;

				// Reset VM

				vm->stackPtrBeg = vm->stackPtrCur = nullptr;
				vm->tokenPtrBeg = vm->tokenPtrCur = nullptr;

				// Run input

				parseSrc(vm, src, false);

				printObj(env::run(vm), true);

				clear(vm);
				 
			#if DEBUG_ALLOC
				std::cout << "have " << vm->objPool->count() << " objects and " << vm->nodePool->count() << " nodes\n";
			#endif
			}
		}

		// leaking memory - prevVal
		Obj* beginScope(VM* vm, Node* params, std::function<Obj* (VM*)> body) {
			Node* prevVal = listCpy(vm, params, [](VM* vm, Obj* obj) {
				
				Obj* ret = nullptr;

				if (obj->type == Type::LIST) {
					ret = setObj(vm, env::newObj(vm), obj->_args->val);
					setObj(vm, obj->_args->val, evalObj(vm, obj->_args->nxt->val));
				}
				else {
					ret = setObj(vm, env::newObj(vm), obj);
					setObj(vm, obj, NIL);
				}
				
				return ret;
			});

			Obj* ret = body(vm);

			listForEach(vm, params, [&prevVal](VM* vm, Node* node) {
				if (node->val->type == Type::LIST) {
					setObj(vm, node->val->_args->val, prevVal->val);
				}
				else {
					setObj(vm, node->val, prevVal->val);
				}

				prevVal = prevVal->nxt;
			});

			return ret;
		}

		void clear(VM* vm) {
			vm->stackPtrCur = vm->stackPtrBeg;

			while (vm->stackPtrCur) {

				vm->nodePool->release(vm->stackPtrCur);

				vm->stackPtrCur = vm->stackPtrCur->nxt;
			}
		}

		///////////////////////
		/////Miscellaneous/////
		///////////////////////
		
		void addLib(VM* vm, void(*lib)(VM* vm)) {
			vm->libs.emplace_back(lib);

			lib(vm);
		}

		void addScript(VM* vm, const str& path) {
			jts::parseSrc(vm, readSrc(vm, path));
		}

		void assert(bool cond, const str& mes, State warnType) {
			if (!cond) return;

			switch (warnType) {
			case State::MES:

				std::cout << "MESSAGE: " << mes;
				break;

			case State::WRN:

				std::cout << "WARING: " << mes;
				break;

			case State::ERR:

				std::cout << "ERROR: " << mes;
				exit(EXIT_FAILURE);
			}
		}
	}
}