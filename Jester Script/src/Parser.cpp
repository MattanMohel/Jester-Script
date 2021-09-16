#include "Parser.h"
#include "Token.h"
#include "Object.h"
#include "StrCon.h"
#include "VM.h"

#include "IterableStack.h"

namespace jts
{
	void ParseTokens(VM* vm)
	{
		/*
			ObjNode nodes: 'args' points to the arguments and 'next' to the next function

			EX:

			(set x (+ 5 5))
			(println x)

			translates to:

			(set) --> next: (println) --> next: (null)
			   `--> args: (x +)  `--> args: (x)
			                  `--> args: 5, 5
		*/

		ObjNode** head = &vm->stackPtrBeg;
		ItStack<ObjNode**> funcHead;

		Tok* it = vm->tokenPtrBeg;

		bool onInvocation = false;

		while (it->spec != Spec::NIL)
		{
			if (it->spec == Spec::FLAG)
			{
				it->next->flag += it->flag;

				it = it->next;
				continue;
			}
			// Mark next head as a call
			if (it->spec == Spec::PARENTH_L)
			{
				onInvocation = true;

				funcHead.emplace(head);

				it = it->next;
				continue;
			}
			// Return head to previous function head
			if (it->spec == Spec::PARENTH_R)
			{
				head = &(*funcHead.pop())->next;

				it = it->next;
				continue;
			}

			switch (it->spec)
			{
				case Spec::SYMBOL:

					if (env::GetSymbol(vm, it) == nullptr)
					{
						if (it->flag.Get(SFlag::REF))
						{
							vm->symbols.emplace(it->value, nullptr);
						}
						else
						{
							vm->symbols.emplace(it->value, new Obj());
						}

						vm->symbols[it->value]->flag = it->flag;
					}

					(*head) = new ObjNode(env::GetSymbol(vm, it));

					break;

				case Spec::VALUE:

					(*head) = new ObjNode(TokToLtrl(it));

					break;

				case Spec::CALL:

					Obj* call = new Obj {Type::NIL, Spec::CALL, FnType::NATIVE};
					call->_native = env::GetSymbol(vm, it)->_native; 

					(*head) = new ObjNode(call);

					break;
			}

			(*head)->value->symbol = &it->value;

			// Invocation --> emplace to args
			if (onInvocation)
			{
				head = &(*head)->args;
				onInvocation = false;
			}
			// Otherwise --> emplace to next
			else
			{
				head = &(*head)->next;
			}

			it = it->next;
		}
	}
}