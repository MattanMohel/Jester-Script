#ifndef LISTS_H
#define LISTS_H

#include "../src/Types.h"
#include "../src/Object.h"
#include "../src/Operations.h"
#include "../src/Execute.h"
#include "../src/VM.h"

using namespace jts;

namespace lib
{
	inline void ListsLib(VM* vm)
	{

		// (iterate variable list body)
		env::addSymbol(vm, "iterate", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			auto* list = evalObj(args->next->value, eval)->_args;
			auto* block = args->next->next;

			while (list)
			{
				binaryOp<Binary::SET>(evalObj(args->value, eval), evalObj(list->value, eval));

				while (block->next)
				{
					evalObj(block->value, eval);
					block = block->next;
				}

				Obj* loopRet = evalObj(block->value, eval);

				if (!list->next) return loopRet;

				list = list->next;
				block = args->next->next;
			}
		}));

		// (head list)
		env::addSymbol(vm, "head", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			Obj* head = evalObj(args->value, eval);

			if (!head) return NIL;

			return head->_args->value;
		}));

		// (tail list)
		env::addSymbol(vm, "tail", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			Obj* head = evalObj(args->value, eval);

			if (!head) return NIL;

			auto* elem = head->_args;

			while (elem->next)
			{
				elem = elem->next;
			}

			return elem->value;
		}));

		// (nth index list)
		env::addSymbol(vm, "nth", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			Obj* head = evalObj(args->next->value, eval);

			if (!head) return NIL;

			auto* elem = head->_args;

			for (size_t i = 0; i < castObj<j_int>(evalObj(args->value, eval)); ++i)
			{
				elem = elem->next;

				if (!elem) return NIL;
			}

			return elem->value;
		}));

		// (size list)
		env::addSymbol(vm, "size", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			Obj* head = evalObj(args->value, eval);

			if (!head) return NIL;

			auto* elem = head->_args;
			j_int size = 0;

			while (elem)
			{
				++size;
				elem = elem->next;
			}

			return setTo(ret, size);
		}));

		// (prepend value list)
		env::addSymbol(vm, "prepend", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			Obj* head = evalObj(args->next->value, eval);

			if (!head) return NIL;

			if (!head->_args)
			{
				head->_args = env::acquireNode();
				return binaryOp<Binary::SET>(head->_args->value, evalObj(args->value, eval));
			}

			auto* cur = head->_args;

			head->_args = env::acquireNode();
			head->_args->next = cur;

			return binaryOp<Binary::SET>(head->_args->value, evalObj(args->value, eval));
		}));

		// (append value list)
		env::addSymbol(vm, "append", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			Obj* head = evalObj(args->next->value, eval);


			if (!head) return NIL;

			if (!head->_args)
			{
				head->_args = env::acquireNode();
				return binaryOp<Binary::SET>(head->_args->value, evalObj(args->value, eval));
			}

			auto* elem = head->_args;

			while (elem->next)
			{
				elem = elem->next;
			}

			elem->next = env::acquireNode();

			return binaryOp<Binary::SET>(elem->next->value, evalObj(args->value, eval));
		}));

		// (insert index value list)
		env::addSymbol(vm, "insert", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			Obj* head = evalObj(args->next->next->value, eval);

			if (!head) return NIL;

			if (castObj<j_int>(evalObj(args->value, eval)) <= 0)
			{
				auto* cur = head->_args;
				head->_args = env::acquireNode();
				head->_args->next = cur;

				return binaryOp<Binary::SET>(head->_args->value, evalObj(args->next->value, eval));
			}

			auto* elem = head->_args;

			for (size_t i = 1; i < castObj<j_int>(evalObj(args->value, eval)); ++i)
			{
				elem = elem->next;

				if (!elem) return NIL;
			}

			auto* prev = elem->next;
			elem->next = env::acquireNode();
			elem->next->next = prev;

			return binaryOp<Binary::SET>(elem->next->value, evalObj(args->next->value, eval));
		}));
	}
}

#endif