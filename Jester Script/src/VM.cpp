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

		void AddNative(VM* vm, str value, Obj* (*native)(ObjNode* params))
		{
			vm->natives.emplace(value, native);
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