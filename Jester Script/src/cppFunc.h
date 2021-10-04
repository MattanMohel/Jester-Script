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
	// Iterates bridge arguments
	// --size_t I keeps track of variadic expansion
	inline ObjNode* NextArg(ObjNode*& arg, size_t I)
	{
		arg = arg->next;

		return arg;
	}

	struct CppFn
	{
		virtual Obj* Call(Obj* ret, ObjNode* args) = 0;
	};

	template<typename Ret, typename... Args>
	struct CppFn_Impl : public CppFn
	{
		// Func type
		using Fn = Ret(*)(Args...);

		void Init(Fn func)
		{
			this->func = func;

			paramVec.resize(sizeof...(Args), nullptr);
		}

		Obj* Call(Obj* ret, ObjNode* args) override
		{
			return Call_Impl(ret, args, std::make_index_sequence<sizeof...(Args)>());
		}

		template<size_t... I>
		Obj* Call_Impl(Obj* ret, ObjNode* args, std::index_sequence<I...>)
		{
			for (Obj*& arg : paramVec)
			{
				arg = EvalObj(args->next->value);
				args = args->next;
			}

			// If func is void, evaluate and return NIL
			if constexpr (std::is_void<Ret>::value)
			{
				func(CastObj<Args>(paramVec[I])...);
				return SetTo(ret, nullptr_t());
			}
			else
			{
				// If func isn't void, return the func value
				return SetTo(ret, func(CastObj<Args>(paramVec[I])...));
			}
		}

		std::vector<Obj*> paramVec;
		Fn func;
	};

	namespace env
	{
		// Add a bridge symbol
		template<typename Ret, typename... Args>
		Obj* AddBridge(Ret(*func)(Args...))
		{
			Obj* obj = new Obj { Type::CPP_FN, Spec::SYMBOL };

			auto* cppPtr = new CppFn_Impl<Ret, Args...>();
			cppPtr->Init(func);

			obj->_cppFunc = (CppFn*)cppPtr;

			return obj;
		}
	}
}

#endif