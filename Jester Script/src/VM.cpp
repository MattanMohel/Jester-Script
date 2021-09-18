#include "VM.h"
#include "Token.h"
#include "Execute.h"
#include "Object.h"

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
			Obj* obj = new Obj { Type::CHAR, Spec::VALUE };
			obj->flag.Set(SFlag::CONST, true);
			obj->_char = value;

			return obj;
		}

		template<> Obj* AddConst(bool value)
		{
			Obj* obj = new Obj { Type::BOOL, Spec::VALUE };
			obj->flag.Set(SFlag::CONST, true);
			obj->_bool = value;

			return obj;
		}

		template<> Obj* AddConst(int  value)
		{
			Obj* obj = new Obj { Type::INT, Spec::VALUE };
			obj->flag.Set(SFlag::CONST, true);
			obj->_int = value;

			return obj;
		}

		template<> Obj* AddConst(float value)
		{
			Obj* obj = new Obj { Type::FLOAT, Spec::VALUE };
			obj->flag.Set(SFlag::CONST, true);
			obj->_float = value;

			return obj;
		}

		template<> Obj* AddConst(std::nullptr_t value)
		{
			Obj* obj = new Obj { Type::NIL, Spec::NIL, FnType::NIL };
			obj->flag.Set(SFlag::CONST, true);

			return obj;
		}

		Obj* GetSymbol(VM* vm, Tok* tok)
		{
			if (vm->symbols.find(tok->value) != vm->symbols.end())
			{
				return vm->symbols[tok->value];
			}

			return nullptr;
		}

		void RunVM(VM* vm)
		{
			vm->stackPtrCur = vm->stackPtrBeg;

			while (vm->stackPtrCur)
			{
				EvalObj(vm->stackPtrCur);

				vm->stackPtrCur = vm->stackPtrCur->next;
			}
		}
}}