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

		size_t prnth_depth = 0;
		 
		ObjNode** head = &vm->stackPtrBeg;
		stack_itr<ObjNode**> funcHead;

		Tok* it = vm->tokenPtrBeg;

		// iterate over Tokens
		while (it->spec != Spec::NIL) {
			switch (it->spec) {

			case Spec::LST_BEG:

				(*head) = new ObjNode(new Obj{ Type::LIST, Spec::VALUE });

				break;

			case Spec::SYMBOL:

				if (!env::getSymbol(vm, it->symbol) || env::canShadow(vm, it->symbol)) {
					env::addSymbol(vm, it->symbol, new Obj{ Type::NIL, Spec::SYMBOL });
				}

				(*head) = new ObjNode(env::getSymbol(vm, it->symbol));
				(*head)->value->symbol = it->symbol;

				break;

			case Spec::VALUE:

				(*head) = new ObjNode(tokToLtrl(it));
				(*head)->value->symbol = it->symbol;

				break;
			}

			switch (it->spec) {

			case Spec::LST_BEG:

				if (it->symbol == "[") {
					SymbolMap* scope;

					if (!vm->lexicalScope) {
						scope = vm->scopes.emplace_back(new SymbolMap());
					}
					else {
						scope = new SymbolMap();
						vm->lexicalScope->next = scope;
						scope->prev = vm->lexicalScope;
					}

					scope->prnth_depth = prnth_depth;
					scope->open = true;

					vm->lexicalScope = scope;
				}
	
				++prnth_depth;

				// set next to the list's argument node
				funcHead.emplace(head);
				head = &(*head)->value->_args;
				break;

			case Spec::LST_END:

				--prnth_depth;

				if (it->symbol == "]") {
					env::assert(!vm->lexicalScope, "closed a closure without opening one");
					vm->lexicalScope->open = false;
				}

				if (vm->lexicalScope && vm->lexicalScope->prnth_depth > prnth_depth) {
					if (vm->lexicalScope) {
						vm->lexicalScope = vm->lexicalScope->prev;
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