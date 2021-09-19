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
		bool isQuoted = false;

		while (it->spec != Spec::NIL)
		{
			switch (it->spec)
			{
				case Spec::FLAG:
					
					it->next->flag += it->flag;

					it = it->next;
					continue;

				case Spec::QUOTE:

					isQuoted = true;

					it = it->next;
					continue;

				// Mark next head as a call
				case Spec::PARENTH_L:
					
					onInvocation = true;

					funcHead.emplace(head);

					it = it->next;
					continue;

				// Return head to previous function head
				case Spec::PARENTH_R:

					head = &(*funcHead.pop())->next;

					it = it->next;
					continue;
			}

			switch (it->spec)
			{
				case Spec::SYMBOL:

					if (env::GetSymbol(vm, it) == nullptr)
					{
						vm->symbols.emplace(it->value, new Obj());

						vm->symbols[it->value]->flag = it->flag;
					}

					(*head) = new ObjNode(env::GetSymbol(vm, it), onInvocation ,isQuoted);

					break;

				case Spec::LTRL:

					(*head) = new ObjNode(TokToLtrl(it), onInvocation, isQuoted);

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

			isQuoted = false;

			it = it->next;
		}
	}
}