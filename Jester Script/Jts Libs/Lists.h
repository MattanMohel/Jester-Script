#ifndef LISTS_H
#define LISTS_H

#include "../src/Types.h"
#include "../src/Object.h"
#include "../src/Operations.h"
#include "../src/Execute.h"
#include "../src/VM.h"

using namespace jts;

namespace lib {
	inline void ListsLib(VM* vm) {

		// (iterate variable list body)
		env::addSymbol(vm, "iterate", env::addNative([](VM* vm, Obj* ret, ObjNode* args, bool eval)
		{
			auto list = evalObj(vm, args->next->value, eval)->_args;
			auto block = args->next->next;

			while (list) {
				set(vm, evalObj(vm, args->value, eval), evalObj(vm, list->value, eval));

				while (block->next) {
					evalObj(vm, block->value, eval);
					block = block->next;
				}

				auto loopRet = evalObj(vm, block->value, eval);

				if (!list->next) {
					set(vm, ret, loopRet);
					break;
				}

				list = list->next;
				block = args->next->next;
			}
		}));

		// (head list)
		env::addSymbol(vm, "head", env::addNative([](VM* vm, Obj* ret, ObjNode* args, bool eval) 
		{
			Obj* head = evalObj(vm, args->value, eval);

			if (!head) {
				set(vm, ret, NIL);
				return;
			}
				
			set(vm, ret, head->_args->value);
		}));

		// (tail list)
		env::addSymbol(vm, "tail", env::addNative([](VM* vm, Obj* ret, ObjNode* args, bool eval) 
		{
			Obj* head = evalObj(vm, args->value, eval);

			if (!head) {
				set(vm, ret, NIL);
				return;
			}

			auto* elem = head->_args;

			while (elem->next) {
				elem = elem->next;
			}

			set(vm, ret, elem->value);
		}));

		// (nth index list)
		env::addSymbol(vm, "nth", env::addNative([](VM* vm, Obj* ret, ObjNode* args, bool eval) 
		{
			Obj* head = args->next->value;

			if (!head) {
				set(vm, ret, NIL);
				return;
			}
			
			auto* elem = head->_args;

			for (size_t i = 0; i < castObj<j_int>(evalObj(vm, args->value, eval)); ++i) {
				elem = elem->next;

				if (!elem) {
					set(vm, ret, NIL);
					return;
				}
			}

			set(vm, ret, elem->value);
		}));	
		
		// (seth index value list)
		env::addSymbol(vm, "seth", env::addNative([](VM* vm, Obj* ret, ObjNode* args, bool eval) 
		{
			Obj* head = args->next->next->value;

			if (!head) {
				set(vm, ret, NIL);
				return;
			}
			
			auto* elem = head->_args;

			for (size_t i = 0; i < castObj<j_int>(evalObj(vm, args->value, eval)); ++i) {
				elem = elem->next;

				if (!elem) {
					set(vm, ret, NIL);
					return;
				}
			}

			set(vm, ret, set(vm, elem->value, evalObj(vm, args->next->value, eval)));
		}));

		// (size list)
		env::addSymbol(vm, "size", env::addNative([](VM* vm, Obj* ret, ObjNode* args, bool eval) 
		{
			Obj* head = evalObj(vm, args->value, eval);

			if (!head) {
				set(vm, ret, NIL);
				return;
			}

			auto* elem = head->_args;
			j_int size = 0;

			while (elem) {
				++size;
				elem = elem->next;
			}

			setTo<j_int>(ret, size);
		}));

		// (prepend value list)
		env::addSymbol(vm, "prepend", env::addNative([](VM* vm, Obj* ret, ObjNode* args, bool eval) 
		{
			Obj* head = evalObj(vm, args->next->value, eval);

			if (!head) {
				set(vm, ret, NIL);
				return;
			}

			if (!head->_args) {
				head->_args = env::acquireNode(vm);
				set(vm, head->_args->value, evalObj(vm, args->value, eval));
				return;
			}

			auto* cur = head->_args;

			head->_args = env::acquireNode(vm);
			head->_args->next = cur;

			set(vm, ret, set(vm, head->_args->value, evalObj(vm, args->value, eval)));
		}));

		// (append value list)
		env::addSymbol(vm, "append", env::addNative([](VM* vm, Obj* ret, ObjNode* args, bool eval) 
		{
			Obj* head = evalObj(vm, args->next->value, eval);

			if (!head) {
				set(vm, ret, NIL);
				return;
			}

			if (!head->_args) {
				head->_args = env::acquireNode(vm);
				set(vm, head->_args->value, evalObj(vm, args->value, eval));
				return;
			}

			auto* elem = head->_args;

			while (elem->next) {
				elem = elem->next;
			}

			elem->next = env::acquireNode(vm);

			set(vm, ret, set(vm, elem->next->value, evalObj(vm, args->value, eval)));
		}));

		// (insert index value list)
		env::addSymbol(vm, "insert", env::addNative([](VM* vm, Obj* ret, ObjNode* args, bool eval) 
		{
			Obj* head = evalObj(vm, args->next->next->value, eval);

			if (!head) {
				set(vm, ret, NIL);
				return;
			}

			if (castObj<j_int>(evalObj(vm, args->value, eval)) <= 0) {
				auto* cur = head->_args;
				head->_args = env::acquireNode(vm);
				head->_args->next = cur;

				set(vm, head->_args->value, evalObj(vm, args->next->value, eval));
				return;
			}

			auto* elem = head->_args;

			for (size_t i = 1; i < castObj<j_int>(evalObj(vm, args->value, eval)); ++i) {
				elem = elem->next;

				if (!elem) {
					set(vm, ret, NIL);
					return;
				}
			}

			auto* prev = elem->next;
			elem->next = env::acquireNode(vm);
			elem->next->next = prev;

			set(vm, ret, set(vm, elem->next->value, evalObj(vm, args->next->value, eval)));
		}));
	}
}

#endif