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

		while (it->spec != Spec::NIL)
		{
 		    switch (it->spec)
			{
				case Spec::CALL_BEG:

					(*head) = new ObjNode(Spec::CALL_BEG);

					break;

				case Spec::SYMBOL:

					if (env::GetSymbol(vm, it->value) == nullptr)
					{
						vm->symbols.emplace(it->value, new Obj());

						vm->symbols[it->value]->flag = it->flag;
					}

					(*head) = CreateNode(env::GetSymbol(vm, it->value), it->value);

					break;

				case Spec::VALUE:

					(*head) = CreateNode(TokToLtrl(it), it->value);

					break;
			}

			switch (it->spec)
			{
				case Spec::CALL_BEG:
					
					funcHead.emplace(head);
					head = &(*head)->args;
					break;

				case Spec::CALL_END:
					
					head = &(*funcHead.pop())->next;
					break;

				default:
					
					head = &(*head)->next;
					break;

			}
			
			it = it->next;
		}
	}
}