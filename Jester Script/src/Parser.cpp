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

			it = it->next;

			ParseTokens_Impl(vm, vm->stackPtrCur, it, codeDepth, &vm->stackPtrCur->args);

			vm->stackPtrCur->next = new ObjNode();
			vm->stackPtrCur = vm->stackPtrCur->next;

			it = it->next;
		}
	}

	void ParseTokens_Impl(VM* vm, ObjNode* head, Tok*& it, int& codeDepth, ObjNode** nextNode)
	{
		head->value->type = it->type;
		head->value->spec = it->spec;
		head->value->token = it;

		// For now all funcs are natives
		head->value->fnType = FnType::NATIVE;
		head->value->_native = vm->natives[it->value]->_native;

		int targetDepth = codeDepth - 1;
		
		while (codeDepth > targetDepth)
		{
			it = it->next;

			switch (it->spec)
			{
				case Spec::PARENTH_L:

					++codeDepth;
					break;

				case Spec::PARENTH_R:

					--codeDepth;
					break;

				case Spec::CALL:

					*nextNode = new ObjNode();
					ParseTokens_Impl(vm, *nextNode, it, codeDepth, &(*nextNode)->args);

					nextNode = &head->args->next;
					break;

				case Spec::VALUE:

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