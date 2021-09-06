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

			Obj* startIndex = vm->stackPtrCur;

			vm->stackPtrCur = ParseTokens_Impl(vm, vm->stackPtrCur, it, codeDepth);

			startIndex->next = vm->stackPtrCur;

			it = it->rest;
		}
	}

	Obj* ParseTokens_Impl(VM* vm, Obj* head, Tok*& it, int& codeDepth)
	{
		head->type = it->type;
		head->spec = it->spec;

		// For now all funcs are natives
		head->fnType = FnType::NAT_FUNC;
		head->_native = vm->natives[it->value];

		int targetDepth = codeDepth - 1;

		while (codeDepth > targetDepth)
		{
			Obj** nextNode = nullptr;

			if (head->spec == Spec::FUNC)
			{
				nextNode = &head->args;
			}
			else
			{
				nextNode = &head->next;
			}

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

					*nextNode = new Obj();
					head = ParseTokens_Impl(vm, head->next, it, codeDepth);
					break;

				case Spec::LTRL:

					*nextNode = TokToLtrl(it);
					head = *nextNode;
					break;

				case Spec::SYMBOL:

					if (env::GetSymbol(vm, it) == nullptr)
					{
						vm->symbols.emplace(it->value, new Obj());
					}

					*nextNode = env::GetSymbol(vm, it);
					head = *nextNode;
					break;
			}
		}

		return head;
	}
}