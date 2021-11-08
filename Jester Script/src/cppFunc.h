#ifndef CPP_FUNC_H
#define CPP_FUNC_H

#include "Types.h"
#include "Object.h"
#include "Execute.h"
#include "Operations.h"
#include "Log.h"

#include <vector>

namespace jts
{
	/*
	  A templated function used to invoke C++ functions in JTS

	  How It Works:

	  An instance of CppFn_Impl stores a designated function pointer

	  Upon function invocation...
	  1) a linked list of objects is converted into a vector of objects
	  2) the vector outputs parameters to the function pointer one by one
	  3) the function value is returned
	*/

	struct CppFn
	{
		virtual Obj* call(Obj* ret, ObjNode* args) = 0;
	};

	template<typename Ret, typename... Args>
	struct CppFn_Impl : public CppFn
	{
		using Fn = Ret(*)(Args...);

		Obj* call(Obj* ret, ObjNode* args) override
		{
			return call_Impl(ret, args, std::make_index_sequence<sizeof...(Args)>());
		}

		template<size_t... I>
		Obj* call_Impl(Obj* ret, ObjNode* args, std::index_sequence<I...>)
		{
			std::vector<Obj*> paramVec;

			// create parameter vector

			while (args->next)
			{
				paramVec.emplace_back(evalObj(args->next->value));
				args = args->next;
			}

			if constexpr (std::is_void<Ret>::value)	// func is void, evaluate and return NIL
			{
				func(castObj<Args>(paramVec[I])...);
				return setTo(ret, nullptr_t());
			}
			else // return the func value
			{
				return setTo(ret, func(castObj<Args>(paramVec[I])...));
			}
		}

		Fn func;
	};

	namespace env
	{
		// Add a bridge symbol
		template<typename Ret, typename... Args>
		Obj* addFunction(Ret(*func)(Args...))
		{
			Obj* obj = new Obj { Type::CPP_FN, Spec::SYMBOL };

			auto* cppPtr = new CppFn_Impl<Ret, Args...>();
			cppPtr->func = func;

			obj->_cppFn = (CppFn*)cppPtr;

			return obj;
		}
	}
}

#endif