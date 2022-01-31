#ifndef SCOPE_HELPER_H
#define SCOPE_HELPER_H

#include "core/Types.h"
#include "core/Object.h"
#include "core/VM.h"

#include "ListOp.h"

namespace jts {

	/*
	* a scoped wrapper (static/dynamic) for evaluated Obj values
	* 
	* the wrapper encapsulates evaluated values and releases
	* them in the destructor
	*/

#define WRAPPER_S(ID, ...) Obj* arr[] = {__VA_ARGS__}; wrapper_s<sizeof(arr)/sizeof(void*)> ID(vm, arr)

	template<size_t SZ>
	struct wrapper_s {

		wrapper_s(VM* vm, Obj** value)
			: _arr(value), _vm(vm) {}

		~wrapper_s() {
			for (size_t i = 0; i < SZ; ++i) {
				env::releaseObj(_vm, _arr[i]);
			}
		}

		inline Obj* get(size_t idx) {
			return _arr[idx];
		}

		VM* _vm;
		Obj** _arr;
	};

#define WRAPPER(ID, ...) wrapper ID(vm, {__VA_ARGS__});

	struct wrapper {

		wrapper(VM* vm, const std::initializer_list<Obj*>& value)
			: _vec(value), _vm(vm) {}

		~wrapper() {
			for (size_t i = 0; i < _vec.size(); ++i) {
				env::releaseObj(_vm, _vec[i]);
			}
		}

		inline void push(const std::initializer_list<Obj*>& value) {
			_vec.insert(_vec.end(), value.begin(), value.end());
		}

		inline Obj* get(size_t idx) {
			return _vec[idx];
		}

		VM* _vm;
		std::vector<Obj*> _vec;
	};

#define BINDING_S(ID, ARGS) binding ID(vm, ARGS);

#define BINDING(ID, ARGS, VALS) binding ID(vm, ARGS, VALS);

	struct binding {

		binding(VM* vm, Node* args) 
			: _vm(vm) {

			lst::forEach(vm, args, [&](VM* vm, Obj* elm) {
				if (elm->type == Type::LIST) {
					_prv_pairs.emplace_back(elm->_args->val, env::newObj(vm, elm->_args->val));
					setObj(vm, elm->_args->val, evalObj(vm, elm->_args->nxt->val));
				}
				else {
					_prv_pairs.emplace_back(elm, env::newObj(vm, elm));
					setTo(elm, nullptr);
				}
			});
		}		
		
		binding(VM* vm, Node* args, Node* vals) 
			: _vm(vm) {

			lst::forEach(vm, args, [&](VM* vm, Obj* elm) {
				_prv_pairs.emplace_back(elm, env::newObj(vm, elm));
				setObj(vm, elm, vals->val);

				shift(&vals);
			});
		}

		~binding() {
			for (auto pair : _prv_pairs) {
				setObj(_vm, pair.first, pair.second);
				env::releaseObj(_vm, pair.second);
			}
		}

		VM* _vm;
		std::vector<std::pair<Obj*, Obj*>> _prv_pairs;
	};
}

#endif
