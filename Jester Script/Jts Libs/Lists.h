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
		env::addSymbol(vm, "iterate", env::addNative([](Obj* ret, ObjNode* args, bool eval)
		{
			auto list = evalObj(args->next->value, eval)->_args;
			auto block = args->next->next;

			while (list) {
				binaryOp<Binary::SET>(evalObj(args->value, eval), evalObj(list->value, eval));

				while (block->next) {
					evalObj(block->value, eval);
					block = block->next;
				}

				auto loopRet = evalObj(block->value, eval);

				if (!list->next) {
					binaryOp<Binary::SET>(ret, loopRet);
					break;
				}

				list = list->next;
				block = args->next->next;
			}
		}));

		// (head list)
		env::addSymbol(vm, "head", env::addNative([](Obj* ret, ObjNode* args, bool eval) 
		{
			Obj* head = evalObj(args->value, eval);

			if (!head) {
				binaryOp<Binary::SET>(ret, NIL);
				return;
			}
				
			binaryOp<Binary::SET>(ret, head->_args->value);
		}));

		// (tail list)
		env::addSymbol(vm, "tail", env::addNative([](Obj* ret, ObjNode* args, bool eval) 
		{
			Obj* head = evalObj(args->value, eval);

			if (!head) {
				binaryOp<Binary::SET>(ret, NIL);
				return;
			}

			auto* elem = head->_args;

			while (elem->next) {
				elem = elem->next;
			}

			binaryOp<Binary::SET>(ret, elem->value);
		}));

		// (nth index list)
		env::addSymbol(vm, "nth", env::addNative([](Obj* ret, ObjNode* args, bool eval) 
		{
			Obj* head = args->next->value;

			if (!head) {
				binaryOp<Binary::SET>(ret, NIL);
				return;
			}
			
			auto* elem = head->_args;

			for (size_t i = 0; i < castObj<j_int>(evalObj(args->value, eval)); ++i) {
				elem = elem->next;

				if (!elem) {
					binaryOp<Binary::SET>(ret, NIL);
					return;
				}
			}

			binaryOp<Binary::SET>(ret, elem->value);
		}));	
		
		// (seth index value list)
		env::addSymbol(vm, "seth", env::addNative([](Obj* ret, ObjNode* args, bool eval) 
		{
			Obj* head = args->next->next->value;

			if (!head) {
				binaryOp<Binary::SET>(ret, NIL);
				return;
			}
			
			auto* elem = head->_args;

			for (size_t i = 0; i < castObj<j_int>(evalObj(args->value, eval)); ++i) {
				elem = elem->next;

				if (!elem) {
					binaryOp<Binary::SET>(ret, NIL);
					return;
				}
			}

			binaryOp<Binary::SET>(ret, binaryOp<Binary::SET>(elem->value, evalObj(args->next->value, eval)));
		}));

		// (size list)
		env::addSymbol(vm, "size", env::addNative([](Obj* ret, ObjNode* args, bool eval) 
		{
			Obj* head = evalObj(args->value, eval);

			if (!head) {
				binaryOp<Binary::SET>(ret, NIL);
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
		env::addSymbol(vm, "prepend", env::addNative([](Obj* ret, ObjNode* args, bool eval) 
		{
			Obj* head = evalObj(args->next->value, eval);

			if (!head) {
				binaryOp<Binary::SET>(ret, NIL);
				return;
			}

			if (!head->_args) {
				head->_args = env::acquireNode();
				binaryOp<Binary::SET>(head->_args->value, evalObj(args->value, eval));
				return;
			}

			auto* cur = head->_args;

			head->_args = env::acquireNode();
			head->_args->next = cur;

			binaryOp<Binary::SET>(ret, binaryOp<Binary::SET>(head->_args->value, evalObj(args->value, eval)));
		}));

		// (append value list)
		env::addSymbol(vm, "append", env::addNative([](Obj* ret, ObjNode* args, bool eval) 
		{
			Obj* head = evalObj(args->next->value, eval);

			if (!head) {
				binaryOp<Binary::SET>(ret, NIL);
				return;
			}

			if (!head->_args) {
				head->_args = env::acquireNode();
				binaryOp<Binary::SET>(head->_args->value, evalObj(args->value, eval));
				return;
			}

			auto* elem = head->_args;

			while (elem->next) {
				elem = elem->next;
			}

			elem->next = env::acquireNode();

			binaryOp<Binary::SET>(ret, binaryOp<Binary::SET>(elem->next->value, evalObj(args->value, eval)));
		}));

		// (insert index value list)
		env::addSymbol(vm, "insert", env::addNative([](Obj* ret, ObjNode* args, bool eval) 
		{
			Obj* head = evalObj(args->next->next->value, eval);

			if (!head) {
				binaryOp<Binary::SET>(ret, NIL);
				return;
			}

			if (castObj<j_int>(evalObj(args->value, eval)) <= 0) {
				auto* cur = head->_args;
				head->_args = env::acquireNode();
				head->_args->next = cur;

				binaryOp<Binary::SET>(head->_args->value, evalObj(args->next->value, eval));
				return;
			}

			auto* elem = head->_args;

			for (size_t i = 1; i < castObj<j_int>(evalObj(args->value, eval)); ++i) {
				elem = elem->next;

				if (!elem) {
					binaryOp<Binary::SET>(ret, NIL);
					return;
				}
			}

			auto* prev = elem->next;
			elem->next = env::acquireNode();
			elem->next->next = prev;

			binaryOp<Binary::SET>(ret, binaryOp<Binary::SET>(elem->next->value, evalObj(args->next->value, eval)));
		}));
	}
}

#endif