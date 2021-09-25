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
			   `--> args: (x, +)  `--> args: (x)
			                  `--> args: (5, 5)
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

					// Create ret value for call instance
					(*head)->value->ret = new ObjNode(new Obj());

					break;

				case Spec::SYMBOL:

					if (!env::GetSymbol(vm, it->value)) vm->symbols.emplace(it->value, new Obj());

					(*head) = new ObjNode(env::GetSymbol(vm, it->value));
					(*head)->value->symbol = it->value;

					(*head)->value->ret = new ObjNode(new Obj());

					break;

				case Spec::VALUE:

					(*head) = new ObjNode(TokToLtrl(it));
					(*head)->value->symbol = it->value;

					(*head)->value->ret = new ObjNode(new Obj());

					break;
			}


			switch (it->spec)
			{
				case Spec::CALL_BEG:
					
					funcHead.emplace(head);
					head = &(*head)->value->_args;
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