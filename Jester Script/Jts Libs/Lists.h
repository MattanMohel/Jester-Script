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

		// (for var list body)
		env::addSymbol(vm, "for", env::addNative([](VM* vm, Node* args)
		{
			auto lst = args->nxt->val->_args;
			auto bdy = args->nxt->nxt;
			auto ret = (Obj*)nullptr;

			while (lst) {
				setObj(vm, args->val, evalObj(vm, lst->val));

				while (bdy->nxt) {
					evalObj(vm, bdy->val);
					bdy = bdy->nxt;
				}

				ret = evalObj(vm, bdy->val);

				lst = lst->nxt;
				bdy = args->nxt->nxt;
			}

			return env::newObj(vm, ret);
		}));

		// (head list)
		env::addSymbol(vm, "fst", env::addNative([](VM* vm, Node* args) 
		{			
			return env::newObj(vm, args->val->_args->val);
		}));

		// (tail list)
		env::addSymbol(vm, "rst", env::addNative([](VM* vm, Node* args) 
		{
			auto ret = env::newObj(vm, Type::LIST, Spec::SYMBOL);
			ret->_args = args->val->_args->nxt;

			return ret;
		}));

		// (nth index list)
		env::addSymbol(vm, "nth", env::addNative([](VM* vm, Node* args) 
		{
			auto lst = args->nxt->val;
			auto elm = lst->_args;
			auto idx = castObj<jtsi>(evalObj(vm, args->val));

			while (idx > 0 && elm) {
				elm = elm->nxt;
				--idx;
			}

			return idx == 0
				? env::newObj(vm, elm->val)
				: env::newObj(vm, NIL);
		}));	

		// (size list)
		env::addSymbol(vm, "size", env::addNative([](VM* vm, Node* args) 
		{
			auto lst = args->val;
			auto elm = lst->_args;
			auto size = 0;

			while (elm) {
				elm = elm->nxt;
				++size;
			}

			return setTo<jtsi>(env::newObj(vm), size);
		}));

		// (prepend value list)
		env::addSymbol(vm, "prepend", env::addNative([](VM* vm, Node* args) 
		{
			auto val = evalObj(vm, args->val);
			auto lst = args->nxt->val;
			auto prvVal = (Obj*)nullptr;
			
			setObj(vm, prvVal, lst->_args->val);
			setObj(vm, lst->_args->val, val);

			auto prvNode = lst->_args->nxt;

			lst->_args->nxt = env::newNode(vm);
			lst->_args->nxt->nxt = prvNode;
			
			return setObj(vm, lst->_args->nxt->val, prvVal);
		}));

		// (append value list)
		env::addSymbol(vm, "append", env::addNative([](VM* vm, Node* args) 
		{
			auto val = evalObj(vm, args->val);
			auto lst = args->nxt->val->_args;

			while (lst->nxt) {
				lst = lst->nxt;
			}

			lst->nxt = env::newNode(vm);

			return setObj(vm, lst->nxt->val, val);
		}));

		// (insert index value list)
		env::addSymbol(vm, "insert", env::addNative([](VM* vm, Node* args) 
		{
			auto val = evalObj(vm, args->nxt->val);
			auto lst =args->nxt->nxt->val;
			auto elm = lst->_args;
			auto idx = castObj<jtsi>(evalObj(vm, args->val));

			if (idx == 0) {
				auto prvVal = (Obj*)nullptr;

				setObj(vm, prvVal, lst->_args->val);
				setObj(vm, lst->_args->val, val);

				auto prvNode = lst->_args->nxt;

				lst->_args->nxt = env::newNode(vm);
				lst->_args->nxt->nxt = prvNode;

				return setObj(vm, lst->_args->nxt->val, prvVal);
			}

			while (idx > 1) {
				elm = elm->nxt;
				--idx;
			}

			auto prv = elm->nxt;
			elm->nxt = env::newNode(vm);
			setObj(vm, elm->nxt->val, val);
			elm->nxt->nxt = prv;

			return env::newObj(vm, val);
		}));
	}
}

#endif