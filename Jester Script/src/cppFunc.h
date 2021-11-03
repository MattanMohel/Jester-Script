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
	struct CppFn
	{
		virtual Obj* call(Obj* ret, ObjNode* args) = 0;
	};

	template<typename Ret, typename... Args>
	struct CppFn_Impl : public CppFn
	{
		// Func type
		using Fn = Ret(*)(Args...);

		Obj* call(Obj* ret, ObjNode* args) override
		{
			return call_Impl(ret, args, std::make_index_sequence<sizeof...(Args)>());
		}

		template<size_t... I>
		Obj* call_Impl(Obj* ret, ObjNode* args, std::index_sequence<I...>)
		{
			std::vector<Obj*> paramVec;

			while (args->next)
			{
				paramVec.emplace_back(evalObj(args->next->value));
				args = args->next;
			}

			// If func is void, evaluate and return NIL
			if constexpr (std::is_void<Ret>::value)
			{
				func(castObj<Args>(paramVec[I])...);
				return setTo(ret, nullptr_t());
			}
			else
			{
				// If func isn't void, return the func value
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