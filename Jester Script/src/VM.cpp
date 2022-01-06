#include "VM.h"
#include "Token.h"
#include "Execute.h"
#include "Object.h"
#include "Lexer.h"
#include "File.h"
#include "Log.h"

#include <iostream>

namespace jts {
	namespace env {

		VM* newVM() {
			VM* vm = new VM();

			vm->objPool = new Pool<Obj>("Object", STARTING_COUNT, [](Obj* value) {
				value->refCount = nullptr;
				value->type = Type::NIL;
				value->_int = 0;

				return value;
			});

			vm->nodePool = new Pool<ObjNode>("Node", STARTING_COUNT, [](ObjNode* value) {
				value->next = nullptr;
				value->value = nullptr;

				return value;
			});

			return vm;
		}

		void addSymbol(VM* vm, str key, Obj* value) {
			bool hasKey = env::getSymbol(vm, key);

			assert(false, "creating duplicate symbol " + key);

			if (vm->curScope && vm->curScope->open) {
				vm->curScope->symbols.emplace(key, value);
			}
			else {
				vm->symbols.emplace(key, value);
			}
		}

		Obj* getSymbol(VM* vm, str symbol) {
			if (vm->curScope) {
				auto scope = vm->curScope;

				while (scope) {
					if (scope->symbols.find(symbol) != scope->symbols.end()) {
						return scope->symbols[symbol];
					}

					scope = scope->prev;
				}
			}

			if (vm->symbols.find(symbol) != vm->symbols.end()) {
				return vm->symbols[symbol]; 
			}

			return nullptr;
		}

		bool symbolExistsOutOfScope(VM* vm, str symbol) {
			return getSymbol(vm, symbol) && vm->curScope && vm->curScope->symbols.find(symbol) == vm->curScope->symbols.end();
		}

		inline void assert(bool cond, str mes, State warnType) {
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

		Obj* run(VM* vm) {
			vm->stackPtrCur = vm->stackPtrBeg;

			while (vm->stackPtrCur->next) {
				evalObj(vm, vm->stackPtrCur->value);

				vm->stackPtrCur = vm->stackPtrCur->next;
			}

			return evalObj(vm, vm->stackPtrCur->value);
		}

		void clear(VM* vm) {
			vm->stackPtrCur = vm->stackPtrBeg;

			while (vm->stackPtrCur) {

				vm->nodePool->release(vm->stackPtrCur);

				vm->stackPtrCur = vm->stackPtrCur->next;
			}
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

				if (src.substr(0, 2) == "--") {
					parseSrc(vm, readSrc(vm, src.substr(2, src.length() - 3)), false);
				}
				else {
					parseSrc(vm, src, false);
				}

				printObj(env::run(vm), true);

				clear(vm);

			#if DEBUG_ALLOC
				std::cout << "have " << vm->objPool->count() << " objects and " << vm->nodePool->count() << " nodes\n";
			#endif
			}
		}

		void addLib(VM* vm, void(*lib)(VM* vm)) {
			vm->libs.emplace_back(lib);

			lib(vm);
		}

		void addScript(VM* vm, const str& path) {
			jts::parseSrc(vm, readSrc(vm, path));
		}

		Obj* addSrcCode(VM* vm, str src) {
			src += EOF;
			parseSrc(vm, src);

			return env::run(vm);
		}

		Obj* addNative(void (*native)(VM*, Obj*, ObjNode*, bool)) {
			Obj* obj = new Obj();

			obj->_native = native;
			obj->type = Type::NAT_FN;
			obj->spec = Spec::SYMBOL;

			obj->constant = true;

			return obj;
		}

		template<> Obj* addConst(j_char value) {
			Obj* obj = new Obj{ Type::CHAR, Spec::SYMBOL };
			obj->constant = true;
			obj->_char = value;

			obj->constant = true;

			return obj;
		}

		template<> Obj* addConst(j_bool value) {
			Obj* obj = new Obj{ Type::BOOL, Spec::SYMBOL };
			obj->constant = true;
			obj->_bool = value;

			obj->constant = true;

			return obj;
		}

		template<> Obj* addConst(j_int  value) {
			Obj* obj = new Obj{ Type::INT, Spec::SYMBOL };
			obj->constant = true;
			obj->_int = value;

			obj->constant = true;

			return obj;
		}

		template<> Obj* addConst(j_float value) {
			Obj* obj = new Obj{ Type::FLOAT, Spec::SYMBOL };
			obj->constant = true;
			obj->_float = value;

			obj->constant = true;

			return obj;
		}

		template<> Obj* addConst(std::nullptr_t value) {
			Obj* obj = new Obj{ Type::NIL, Spec::SYMBOL };

			obj->constant = true;

			return obj;
		}

		ObjNode* acquireNode(VM* vm, Type type, Spec spec) {
			auto* node = vm->nodePool->acquire();
			node->value = vm->objPool->acquire();

			node->value->type = type;
			node->value->spec = spec;

			return node;
		}

		ObjNode* acquireNode(VM* vm, Obj* obj) {
			auto* node = vm->nodePool->acquire();

			node->value = obj;

			return node;
		}

		void releaseNode(VM* vm, ObjNode* node) {
			vm->objPool->release(node->value);
			vm->nodePool->release(node);
		}
	}
}