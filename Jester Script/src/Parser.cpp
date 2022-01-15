#include "Parser.h"
#include "Types.h"
#include "Token.h"
#include "Object.h"
#include "StrCon.h"
#include "VM.h"

namespace jts {

	void parseTokens(VM* vm) {

		/*
			Parses a tokenized source file into a linked-list tree of objects

			Utilizes ObjNode's 'next' value to connect objects together

			EX:

			---------------------
			1: (set x (+ 5 5))
			2: (println x)
			3:
			4: x
			---------------------

			translates to:

			(...) - - - - - - - -> next: (...) - - - - - - -> next: x
			   `--> _args: (set, x, (...))  `--> (println x)
									   `--> _args: (+ 5 5)

			--Everything translates to recursive lists
		*/
		 
		Node** head = &vm->stackPtrBeg;
		stack_itr<Node**> funcHead;

		Tok* it = vm->tokenPtrBeg;

		// iterate over Tokens
		while (it->spec != Spec::NIL) {
			switch (it->spec) {

			case Spec::LST_BEG:

				(*head) = env::newNode(vm, env::newObj(vm, Type::LIST, Spec::VALUE));

				break;

			case Spec::SYMBOL:

				if (!env::getSymbol(vm, it->symbol)) {
					env::addSymbol(vm, it->symbol, env::newObj(vm));
				}

				(*head) = env::newNode(vm, env::getSymbol(vm, it->symbol));
				(*head)->val->symbol = it->symbol;

				break;

			case Spec::VALUE:

				(*head) = env::newNode(vm, tokToLtrl(vm, it));
				(*head)->val->symbol = it->symbol;

				break;
			}

			switch (it->spec) {

			case Spec::LST_BEG:

				// set next to the list's argument node
				funcHead.emplace(head);
				head = &(*head)->val->_args;
				break;

			case Spec::LST_END:

				// set next to the last pushed list head's next node
				head = &(*funcHead.pop())->nxt;
				break;

			default:

				// set next to the next value of the current node
				head = &(*head)->nxt;
				break;

			}

			it = it->next;
		}
	}
}