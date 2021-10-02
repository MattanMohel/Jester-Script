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
			Parses a tokenized source file into a linked-list tree of objects

			Utilizes ObjNode's 'next' value to connect objects together

			EX:

			  ---------------------
			|| 1: (set x (+ 5 5))  ||
			|| 2: (println x)      ||
			|| 3:                  ||
			|| 4: x                ||
			  ---------------------

			translates to:

			(...) - - - - - - - -> next: (...) - - - - - - -> next: x
			   `--> _args: (set, x, (...))  `--> (println x)
									   `--> _args: (+ 5 5)

			--Everything translates to recursive lists
		*/

		ObjNode** head = &vm->stackPtrBeg;
		ItStack<ObjNode**> funcHead;

		Tok* it = vm->tokenPtrBeg;

		// iterate over Tokens
		while (it->spec != Spec::NIL)
		{
 		    switch (it->spec)
			{
				case Spec::HEAD:

					(*head) = new ObjNode(new Obj{ Type::LIST, Spec::HEAD });

					break;

				case Spec::SYMBOL:

					// if symbol id doesn't exist, create it
					if (!env::GetSymbol(vm, it->value)) env::AddSymbol(vm, it->value, new Obj { Type::NIL, Spec::SYMBOL });

					// assign object the value of the corresponding symbol
					(*head) = new ObjNode(env::GetSymbol(vm, it->value));
					(*head)->value->symbol = it->value;

					break;

				case Spec::VALUE:

					// if literal value, parse the symbol to value and assign to object
					(*head) = new ObjNode(TokToLtrl(it));
					(*head)->value->symbol = it->value;

					break;
			}


			switch (it->spec)
			{
				case Spec::HEAD: // '(' value
					
					// set next to the list's argument node
					funcHead.emplace(head);
					head = &(*head)->value->_args;
					break;

				case Spec::END: // ')' value
					
					// set next to the last pushed list head's next node
					head = &(*funcHead.pop())->next;
					break;

				default:
					
					// set next to the next value of the current node
					head = &(*head)->next;
					break;

			}
			
			it = it->next;
		}
	}
}