#ifndef LISTS_H
#define LISTS_H

#include "core/Types.h"
#include "core/Object.h"
#include "core/Execute.h"
#include "core/VM.h"

#include "util/ObjectOp.h"
#include "util/ListOp.h"

using namespace jts;

namespace lib {
	inline void ListsLib(VM* vm) {

		// (for var list body)
		env::addSymbol(vm, "for", env::addNative([](VM* vm, Node* args)
		{
			auto lst = args->nxt->val->_args;
			auto bdy = args->nxt->nxt;
			auto ret = (Obj*)nullptr;

			if (!lst) {
				return env::newObj(vm, NIL);
			}

			while (lst) {
				setObj(vm, args->val, evalObj(vm, lst->val));

				while (bdy->nxt) {
					evalObj(vm, shiftr(&bdy)->val);
				}

				ret = evalObj(vm, bdy->val);

				shift(&lst);
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
		
		// (new list to-set)
		env::addSymbol(vm, "new", env::addNative([](VM* vm, Node* args) 
		{
			auto val = args->nxt->val;

			val->_args = lst::copy(vm, shiftr(&args)->val->_args);
			val->type = Type::LIST;

			return env::newObj(vm, val);
		}));

		// (prepend value list)
		env::addSymbol(vm, "prepend", env::addNative([](VM* vm, Node* args) 
		{
			auto val = evalObj(vm, args->val);
			auto lst = args->nxt->val->_args;
			auto prvVal = env::newObj(vm);

			if (!lst) {
				args->nxt->val->_args = env::newNode(vm,
					env::newObj(vm, val));

				return env::newObj(vm, val);
			}
			
			setObj(vm, prvVal, lst->val);
			setObj(vm, lst->val, val);

			auto prvNode = lst->nxt;

			lst->nxt = env::newNode(vm);
			lst->nxt->nxt = prvNode;
			
			setObj(vm, lst->nxt->val, prvVal);
			return env::newObj(vm, val);
		}));

		// (append value list)
		env::addSymbol(vm, "append", env::addNative([](VM* vm, Node* args) 
		{
			auto val = evalObj(vm, args->val);
			auto lst = args->nxt->val->_args;

			if (!lst) {
				args->nxt->val->_args = env::newNode(vm, 
					env::newObj(vm, val));

				return env::newObj(vm, val);
			}

			while (lst->nxt) {
				lst = lst->nxt;
			}

			lst->nxt = env::newNode(vm);

			return env::newObj(vm, setObj(vm, lst->nxt->val, val));
		}));

		// (insert index value list)
		env::addSymbol(vm, "insert", env::addNative([](VM* vm, Node* args) 
		{
			auto val = evalObj(vm, args->nxt->val);
			auto lst =args->nxt->nxt->val->_args;
			auto idx = castObj<jtsi>(evalObj(vm, args->val));

			while (idx > 0 && lst->nxt) {
				lst = lst->nxt;
				--idx;
			}

			if (idx > 0) {
				lst->nxt = env::newNode(vm, env::newObj(vm));
				return env::newObj(vm, setObj(vm, lst->nxt->val, val));
			}

			auto prvVal = env::newObj(vm);
			setObj(vm, prvVal, lst->val);
			setObj(vm, lst->val, val);

			auto prvNode = lst->nxt;
			lst->nxt = env::newNode(vm, prvVal);
			lst->nxt->nxt = prvNode;

			return env::newObj(vm, val);
		}));		
		
		// (remove value list)
		env::addSymbol(vm, "remove", env::addNative([](VM* vm, Node* args) 
		{
			auto val = evalObj(vm, args->val);
			auto elm = args->nxt->val->_args;
			auto prv = (Node*)nullptr;

			if (!elm) {
				return env::newObj(vm, NIL);
			}

			while (elm->nxt && !isEqual(elm->val, val)) {
				prv =  elm;
				shift(&elm);
			}

			if (elm->nxt) {
				setObj(vm, elm->val, elm->nxt->val);

				elm->nxt = elm->nxt->nxt
					? elm->nxt->nxt
					: nullptr;

				//delete elm->nxt

				return env::newObj(vm, val);
			}
			if (isEqual(elm->val, val)) {
				prv
					? prv->nxt = nullptr
					: args->nxt->val->_args = nullptr;
				
				//delete elm
				//or delete _args

				return env::newObj(vm, val);
			}

			return env::newObj(vm, NIL);
		}));
	}
}

#endif