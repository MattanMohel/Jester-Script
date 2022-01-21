#include "VM.h"
#include "Token.h"
#include "Execute.h"
#include "Object.h"
#include "Lexer.h"
#include "File.h"
#include "Log.h"

#include "util/ObjectOp.h"
#include "util/ListOp.h"

#include <iostream>
#include <filesystem>

namespace jts {
	namespace env {

		//////////////////////
		/////Type Setters/////
		//////////////////////

		VM* newVM() {
			VM* vm = new VM();

			vm->objPool = new Pool<Obj>("Object", START_OBJ_COUNT, [](Obj* val) {
				val->ref  = new size_t(1);
				val->spec = Spec::SYMBOL;
				val->type = Type::NIL;
				val->_int = 0;

				return val;
			});			
			
			vm->cmpl_objPool = new Pool<Obj>("Object", START_OBJ_COUNT, [](Obj* val) {
				val->ref  = new size_t(1);
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
			
			vm->cmpl_nodePool = new Pool<Node>("Node", START_OBJ_COUNT, [](Node* val) {
				val->nxt = nullptr;
				val->val = nullptr;

				return val;
			});

			vm->workDir = file::projectDir();

			return vm;
		}

		VM* setEval(VM* vm, bool state) {
			vm->eval = state;
			return vm;
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

		Obj* newObj(VM* vm, Obj* obj) {
			return setObj(vm, vm->objPool->acquire(), obj, false);
		}

		void releaseObj(VM* vm, Obj* obj) {
			vm->objPool->release(obj);
		}

		Node* cmplNode(VM* vm, Obj* obj) {
			auto* node = vm->cmpl_nodePool->acquire();

			node->val = obj;

			return node;
		}

		Obj* cmplObj(VM* vm, Type t, Spec s) {
			auto obj = vm->cmpl_objPool->acquire();
			obj->type = t;
			obj->spec = s;

			return obj;
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

		Obj* addSrc(VM* vm, const str& src) {
			(*const_cast<str*>(&src)) += EOF;
			file::parseSrc(vm, src);

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

			Obj* ret = nullptr;

			while (vm->stackPtrCur) {
				ret = evalObj(vm, vm->stackPtrCur->val);
				env::releaseUsed(vm);

				shift(&vm->stackPtrCur);
			}

			return ret;
		}

		Obj* runREPL(VM* vm) {
			str src;
			size_t line = 0;

			Obj* ret = nullptr;

			while (1) {
				std::cout << "[" << line++ << "]>> ";
				std::getline(std::cin, src);
				src += EOF;

				if (src.empty()) {
					continue;
				}

				if ("$env parse scripts/jts/Jester.jts")

				// Reset VM

				vm->stackPtrBeg = vm->stackPtrCur = nullptr;
				vm->tokenPtrBeg = vm->tokenPtrCur = nullptr;

				// Run input

				file::parseSrc(vm, src, false);

				ret = printObj(env::run(vm), true);
				 
			#if DEBUG_ALLOC
				std::cout << "have " << vm->objPool->count() << " objects and " << vm->nodePool->count() << " nodes\n";
			#endif
			}

			return ret;
		}

		//MEMORY LEAKS

		Node* pushEnv(VM* vm, Node* locals, Node* newVal) {
			Node* prev = lst::copy(vm, locals);

			auto valPtr = newVal;

			lst::forEach(vm, locals, [&](VM* vm, Obj* elm) {
				setObj(vm, elm, evalObj(vm, shiftr(&valPtr)->val), false);
			});

			//lst::free(vm, newVal);

			return prev;
		}

		Node* pushEnv(VM* vm, Node* locPair) {
	
			Node* prev =
				lst::copy(vm, locPair, [](VM* vm, Obj* elm) {
					return env::newObj(vm, (elm->type == Type::LIST)
						? elm->_args->val
						: elm);
				});

			lst::forEach(vm, locPair, [](VM* vm, Obj* elm) {
				if (elm->type == Type::LIST) {
					setObj(vm, elm->_args->val, evalObj(vm, elm->_args->nxt->val), 
						false);
				}
				else {
					setObj(vm, elm, NIL, 
						false);
				}
			});

			return prev;
		}

		void endEnv(VM* vm, Node* locals, Node* prvVal) {

			auto valPtr = prvVal;
			
			lst::forEach(vm, locals, [&](VM* vm, Obj* elm) {
				if (elm->type == Type::LIST) {
					setObj(vm, elm->_args->val, shiftr(&valPtr)->val,
						false);
				}
				else {
					setObj(vm, elm, shiftr(&valPtr)->val,
						false);
				}
			});

			//lst::free(vm, prvVal);
		}

		void pushUsed(VM* vm, Obj* ret) {
			vm->objPool->push_used(ret);
		}

		void releaseUsed(VM* vm) {
			vm->objPool->release_used();
		}
		void clear(VM* vm) {
			vm->stackPtrCur = vm->stackPtrBeg;

			while (vm->stackPtrCur) {

				vm->nodePool->release(vm->stackPtrCur);

				vm->stackPtrCur = vm->stackPtrCur->nxt;
			}
		}

		///////////////////
		/////Directory/////
		///////////////////

		const str& getDir(VM* vm) {
			return vm->workDir;
		}

		void changeDir(VM* vm, const str& cd) {
			if (cd[0] == '\\') {
				vm->workDir = cd.substr(1);
			}
			else if (cd[0] == '.') {
				size_t fst = cd.find_first_of('\\');

				str cond = cd.substr(0, fst);
				str path = cd.substr(fst + 1);

				size_t count = cond.find_last_of('.');

				// "C:/Downlaods/Secret"

				for (size_t i = 0; i <= count; ++i) {
					size_t idx = vm->workDir.find_last_of("\\");

					ASSERT(idx == (size_t) - 1, "tried to cd into non existing parent dir " + vm->workDir);

					vm->workDir = vm->workDir.substr(0, idx);
				}

				if (path != "") {
					vm->workDir += "\\" + path;
				}
			}
			else {
				vm->workDir += "\\" + cd;
			}

			ASSERT(!std::filesystem::exists(vm->workDir), "tried to cd into non existing path");
		}

		///////////////////////
		/////Miscellaneous/////
		///////////////////////
		
		void addLib(VM* vm, void(*lib)(VM* vm)) {
			vm->libs.emplace_back(lib);

			lib(vm);
		}

		void addScript(VM* vm, const str& path, bool abs, bool run) {
			if (abs) {
				file::parseSrc(vm, file::readFile(vm, file::open(path)), run);
			}
			else {
				file::parseSrc(vm, file::readFile(vm, file::open(vm, path)), run);
			}
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