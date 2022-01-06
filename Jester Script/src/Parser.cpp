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

		size_t prnthDepth = 0;
		 
		ObjNode** head = &vm->stackPtrBeg;
		stack_itr<ObjNode**> funcHead;

		Tok* it = vm->tokenPtrBeg;

		// iterate over Tokens
		while (it->spec != Spec::NIL) {
			switch (it->spec) {

			case Spec::LST_BEG:

				(*head) = env::acquireNode(vm, Type::LIST, Spec::VALUE);

				break;

			case Spec::SYMBOL:

				// TODO - problem with this (some-fun x [x]...) -- conflicting variables
				// should create a shadow variable for scope only if initialized in []
				if (!env::getSymbol(vm, it->symbol) || env::symbolExistsOutOfScope(vm, it->symbol)) {
					Obj* value = vm->objPool->acquire();
					value->spec = Spec::SYMBOL;
					value->type = Type::NIL;

					env::addSymbol(vm, it->symbol, value);
				}

				(*head) = env::acquireNode(vm, env::getSymbol(vm, it->symbol));
				(*head)->value->symbol = it->symbol;

				break;

			case Spec::VALUE:

				(*head) = env::acquireNode(vm, tokToLtrl(vm, it));
				(*head)->value->symbol = it->symbol;

				break;
			}

			switch (it->spec) {

			case Spec::LST_BEG:

				if (it->symbol == "[") {
					SymbolMap* scope;
					
					if (!vm->curScope) {
						scope = vm->scopes.emplace_back(new SymbolMap());
					}
					else {
						scope = new SymbolMap();
						vm->curScope->next = scope;
						scope->prev = vm->curScope;
					}

					scope->prnthDepth = prnthDepth;
					scope->open = true;

					vm->curScope = scope;
				}
	
				++prnthDepth;

				// set next to the list's argument node
				funcHead.emplace(head);
				head = &(*head)->value->_args;
				break;

			case Spec::LST_END:

				--prnthDepth;

				if (it->symbol == "]") {
					env::assert(!vm->curScope, "closed a closure without opening one");
					vm->curScope->open = false;
				}

				if (vm->curScope && vm->curScope->prnthDepth > prnthDepth) {
					if (vm->curScope) {
						vm->curScope = vm->curScope->prev;
					}
				}

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