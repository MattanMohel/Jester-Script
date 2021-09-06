#include "Parser.h"
#include "Token.h"
#include "Object.h"
#include "StrCon.h"
#include "VM.h"

namespace jts
{
	void ParseTokens(VM* vm)
	{
		int codeDepth = 0;

		Tok* it = vm->tokenPtrBeg;

		vm->stackPtrCur = new Obj();
		vm->stackPtrBeg = vm->stackPtrCur;

		while (it->spec != Spec::NIL)
		{
			++codeDepth;

			it = it->rest;

			ParseTokens_Impl(vm, vm->stackPtrCur, it, codeDepth);

			it = it->rest;
		}
	}

	Obj* ParseTokens_Impl(VM* vm, Obj* head, Tok*& it, int& codeDepth)
	{
		head->type = it->type;
		head->spec = it->spec;

		head->_native = vm->natives[it->value];

		int targetDepth = codeDepth - 1;

		while (codeDepth > targetDepth)
		{
			it = it->rest;

			switch (it->spec)
			{
				case Spec::PARENTH_L:

					++codeDepth;
					break;

				case Spec::PARENTH_R:

					--codeDepth;
					break;

				case Spec::FUNC:

					head->rest = new Obj();
					head = ParseTokens_Impl(vm, head->rest, it, codeDepth);
					break;

				case Spec::LTRL:

					head->rest = TokToLtrl(it); 
					head = head->rest;
					break;

				case Spec::SYMBOL:

					if (env::GetSymbol(vm, it) == nullptr)
					{
						vm->symbols.emplace(it->value, new Obj());
					}

					head->rest = env::GetSymbol(vm, it); 
					head = head->rest;
					break;
			}
		}

		head->argsEnd = true;

		return head;
	}
}