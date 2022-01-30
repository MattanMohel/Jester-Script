#include "VM.h"
#include "Token.h"
#include "Execute.h"
#include "Object.h"
#include "JtsFunc.h"
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

			vm->objPool.init([](Obj* elm) {
				
				elm->ref  = new size_t(1);
				elm->spec =  Spec::SYMBOL;
				elm->type =     Type::NIL;
				elm->_int = 0;

				return elm;
			});			

			vm->nodePool.init([](Node* elm) {
				elm->nxt = nullptr;
				elm->val = nullptr;

				return elm;
			});	

			vm->workingDir = file::getWorkingDir();

			return vm;
		}

		VM* withEval(VM* vm, bool state) {
			vm->eval = state;
			return vm;
		}

		Node* newNode(VM* vm, Obj* obj) {
			Node* node = vm->nodePool.acquire();
			node->val = obj;

			return node;
		}

		void releaseNode(VM* vm, Node* node) {
			vm->nodePool.release(&node);
		}

		Obj* newObj(VM* vm, Type t, Spec s) {
			Obj* obj = vm->objPool.acquire();
			obj->type = t;
			obj->spec = s;

			return obj;
		}

		Obj* newObj(VM* vm, Obj* obj) {
			return setObj(vm, vm->objPool.acquire(), obj, false);
		}

		Obj* newObj(VM* vm, Obj& obj) {
			return setObj(vm, vm->objPool.acquire(), &obj, false);
		}

		void releaseObj(VM* vm, Obj* obj) {
			vm->objPool.release(obj);
		}

		/////////////////
		/////Symbols/////
		/////////////////

		void addSymbol(VM* vm, const str& symbol, Obj* val) {
			bool hasKey = env::getSymbol(vm, symbol);

			ASSERT(false, "creating duplicate symbol " + symbol);

			vm->symbols.emplace(symbol, val);
		}

		Obj* addNative(Obj* (*native)(VM*, Node*)) {
			Obj* obj = new Obj();

			obj->_natFn = new NatFn();
			obj->_natFn->_native = native;
				
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

			while (vm->stackPtrCur->nxt) {
				evalObj(vm, vm->stackPtrCur->val);
				shift(&vm->stackPtrCur);
			}

			return evalObj(vm, vm->stackPtrCur->val);
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

		Node* bindEnv(VM* vm, Node* locals, Node* newVal) {
			Node* prev = lst::copy(vm, locals);

			auto valPtr = newVal;

			lst::forEach(vm, locals, [&](VM* vm, Obj* elm) {
				setObj(vm, elm, evalObj(vm, shiftr(&valPtr)->val), false);
			});

			//lst::free(vm, newVal);

			return prev;
		}

		Node* bindEnv(VM* vm, Node* locPair) {
	
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

		void unbindEnv(VM* vm, Node* locals, Node* prvVal) {

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

		///////////////////
		/////Directory/////
		///////////////////

		const str& pwd(VM* vm) {
			return vm->workingDir;
		}

		str cat(VM* vm, const str& file) {
			return file::readFile(file::open(vm, file)) + '\n';
		}

		void cd(VM* vm, const str& cd) {
			if (cd[0] == '\\') {
				vm->workingDir = cd.substr(1);
			}
			else if (cd[0] == '.') {
				size_t fst = cd.find_first_of('\\');

				str cond = cd.substr(0, fst);
				str path = cd.substr(fst + 1);

				size_t count = cond.find_last_of('.');

				// "C:/Downlaods/Secret"

				for (size_t i = 0; i <= count; ++i) {
					size_t idx = vm->workingDir.find_last_of('\\');

					ASSERT(idx == (size_t) - 1, "tried to cd into non existing parent dir " + vm->workingDir);

					vm->workingDir = vm->workingDir.substr(0, idx);
				}

				if (path != "") {
					vm->workingDir += '\\' + path;
				}
			}
			else {
				vm->workingDir += '\\' + cd;
			}

			ASSERT(!std::filesystem::exists(vm->workingDir), "tried to cd into non existing path");
		}

		str ls(VM* vm) {
			auto files = file::getFiles(vm, "");
			str res = "";

			for (size_t i = 0; i < files.size(); ++i) {
				res += files[i];

				if (i + 1 < files.size()) {
					res += '\n';
				}
			}

			return res;
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
				file::parseSrc(vm, file::readFile(file::open(path)), run);
			}
			else {
				file::parseSrc(vm, file::readFile(file::open(vm, path)), run);
			}
		}
	}
}