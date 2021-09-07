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

		vm->stackPtrCur = new ObjNode();
		vm->stackPtrBeg = vm->stackPtrCur;

		while (it->spec != Spec::NIL)
		{
			++codeDepth;

			it = it->rest;

			ParseTokens_Impl(vm, vm->stackPtrCur, it, codeDepth, &vm->stackPtrCur->args);

			vm->stackPtrCur->next = new ObjNode();
			vm->stackPtrCur = vm->stackPtrCur->next;

			it = it->rest;
		}
	}

	void ParseTokens_Impl(VM* vm, ObjNode* head, Tok*& it, int& codeDepth, ObjNode** nextNode)
	{
		head->value->type = it->type;
		head->value->spec = it->spec;

		// For now all funcs are natives
		head->value->fnType = FnType::NAT_FUNC;
		head->value->_native = vm->natives[it->value];

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

					*nextNode = new ObjNode();
					ParseTokens_Impl(vm, *nextNode, it, codeDepth, &(*nextNode)->args);

					nextNode = &head->args->next;

					break;

				case Spec::LTRL:

					*nextNode = new ObjNode(TokToLtrl(it));
					head = *nextNode;

					nextNode = &head->next;

					break;

				case Spec::SYMBOL:

					if (env::GetSymbol(vm, it) == nullptr)
					{
						vm->symbols.emplace(it->value, new Obj());
					}

					*nextNode = new ObjNode(env::GetSymbol(vm, it));
					head = *nextNode;

					nextNode = &head->next;

					break;
			}
		}
	}
}