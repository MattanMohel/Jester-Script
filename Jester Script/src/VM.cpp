#include "VM.h"
#include "Token.h"
#include "Execute.h"
#include "Object.h"
#include "File.h"
#include "Log.h"

#include <iostream>

namespace jts
{
	namespace env
	{
		void Assert(VM* vm, bool assertion, str message)
		{
			if (assertion)
			{
				// print assertion and quit
			}
		}

		void AddLib(VM* vm, void(*lib)(VM* vm))
		{
			vm->libs.emplace_back(lib);

			lib(vm);
		}

		void AddSymbol(VM* vm, str key, Obj* value)
		{
			vm->symbols.emplace(key, value);
		}

		Obj* AddNative(Obj* (*native)(ObjNode* params))
		{
			Obj* obj = new Obj();
			
			obj->_native = native;
			obj->fnType = FnType::NATIVE;
			obj->spec = Spec::SYMBOL;

			return obj;
		}

		template<> Obj* AddConst(char value)
		{
			Obj* obj = new Obj { Type::CHAR, Spec::SYMBOL };
			obj->flag.Set(SFlag::CONST, true);
			obj->_char = value;

			return obj;
		}

		template<> Obj* AddConst(bool value)
		{
			Obj* obj = new Obj { Type::BOOL, Spec::SYMBOL };
			obj->flag.Set(SFlag::CONST, true);
			obj->_bool = value;

			return obj;
		}

		template<> Obj* AddConst(int  value)
		{
			Obj* obj = new Obj { Type::INT, Spec::SYMBOL };
			obj->flag.Set(SFlag::CONST, true);
			obj->_int = value;

			return obj;
		}

		template<> Obj* AddConst(float value)
		{
			Obj* obj = new Obj { Type::FLOAT, Spec::SYMBOL };
			obj->flag.Set(SFlag::CONST, true);
			obj->_float = value;

			return obj;
		}

		template<> Obj* AddConst(std::nullptr_t value)
		{
			Obj* obj = new Obj { Type::NIL, Spec::SYMBOL, FnType::NIL };
			obj->flag.Set(SFlag::CONST, true);

			return obj;
		}

		Obj* GetSymbol(VM* vm, str symbol)
		{
			if (vm->symbols.find(symbol) != vm->symbols.end()) 
			{
				return vm->symbols[symbol];
			}

			return nullptr;
		}

		void BeginREPL(VM* vm)
		{
			str src;

			size_t inputCount = 0;

			while (1)
			{
				std::cout << "[" << inputCount++ << "]>> ";
				std::getline(std::cin, src);
				src += EOF;

				if (src.empty()) continue;

				ParseSrc(vm, src);

				PrintObj(env::RunVM(vm), true);

				vm->stackPtrBeg = vm->stackPtrCur = nullptr;
				vm->tokenPtrBeg = vm->tokenPtrCur = nullptr;
			}
		}

		ObjNode* RunVM(VM* vm)
		{
			vm->stackPtrCur = vm->stackPtrBeg;

			while (vm->stackPtrCur->next)
			{
				EvalObj(vm->stackPtrCur);

				vm->stackPtrCur = vm->stackPtrCur->next;
			}

			EvalObj(vm->stackPtrCur);
			return vm->stackPtrCur;
		}
}}