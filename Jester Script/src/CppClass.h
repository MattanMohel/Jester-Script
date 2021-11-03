#ifndef CPP_CLASS_H
#define CPP_CLASS_H

#include "Types.h"
#include "Execute.h"
#include "Operations.h"
#include "VM.h"

#define env_ADD_CLASS(vm, type) env::addSymbol(vm, #type, env::addClass<type>());
#define env_ADD_MEMBER(vm, type, member) env::addMember<type>(vm, #type, #member, &type::member);
#define env_ADD_METHOD(vm, type, method) env::addMethod<type>(vm, #type, #method, &type::method);

namespace jts
{
	struct MemberBridge
	{
		virtual void setMember(void* inst, Obj* obj) = 0;

		size_t index = 0;
	};	
	
	template<typename Cls, typename T>
	struct MemberBridge_Impl : public MemberBridge
	{
		using Member = T Cls::*;

		void setMember(void* inst, Obj* obj) override
		{
			((Cls*)inst)->*member = castObj<T>(obj);
		}

		Member member;
	};

	struct MethodBridge
	{
		virtual Obj* call(void* inst, Obj* ret, ObjNode* args) = 0;
	};

	template <typename Cls, typename Ret, typename... Args>
	struct MethodBridge_Impl : public MethodBridge
	{
		using Method = Ret(Cls::*)(Args...);

		Obj* call(void* inst, Obj* ret, ObjNode* args) override
		{
			return call_Impl(inst, ret, args, std::make_index_sequence<sizeof...(Args)>());
		}

		template<size_t... I>
		Obj* call_Impl(void* inst, Obj* ret, ObjNode* args, std::index_sequence<I...>)
		{
			std::vector<Obj*> paramVec;

			while (args->next)
			{
				paramVec.emplace_back(evalObj(args->next->value));
				args = args->next;
			}

			// If method is void, evaluate and return NIL
			if constexpr (std::is_void<Ret>::value)
			{
				(((Cls*)inst)->*method)(castObj<Args>(paramVec[I])...);
				return setTo(ret, nullptr_t());
			}
			else
			{
				// If method isn't void, return the method value
				return setTo(ret, (((Cls*)inst)->*method)(castObj<Args>(paramVec[I])...));
			}
		}

		Method method;
	};

	struct CppClassTemplate
	{
		str typeName;
		std::unordered_map<str, MemberBridge*> members;
		std::unordered_map<str, MethodBridge*> methods;

		size_t memberCount = 0;
	};

	struct CppClass
	{
		virtual Obj* call(str& id, Obj* ret, ObjNode* args) = 0;
		virtual void instance(CppClass*& cppClass) = 0;

		bool hasMember(str& symbol)
		{
			return classTemplate->members.find(symbol) != classTemplate->members.end();
		}

		Obj* getMember(str& symbol)
		{
			return instMembers[classTemplate->members[symbol]->index];
		}
					
		Obj* getMember_NonRef(str symbol)
		{
			return instMembers[classTemplate->members[symbol]->index];
		}

		CppClassTemplate* classTemplate;

		std::vector<Obj*> instMembers;
	};

	template<typename Cls>
	struct CppClass_Impl : public CppClass
	{
		Obj* call(str& id, Obj* ret, ObjNode* args)
		{
			for (auto bridge : classTemplate->members)
			{
				bridge.second->setMember(&inst, instMembers[bridge.second->index]);
			}

			return classTemplate->methods[id]->call(&inst, ret, args);
		}

		void instance(CppClass*& cppClass) override
		{
			CppClass_Impl<Cls>* newClass = new CppClass_Impl<Cls>(); 
			newClass->classTemplate = classTemplate;

			for (auto instMember : instMembers)
			{
				newClass->instMembers.emplace_back(binaryOp<Binary::SET>(new Obj { Type::NIL, Spec::SYMBOL }, instMember));
			}

			cppClass = (CppClass*)newClass;
		}

		Cls inst;
	};

	namespace env
	{
		// Add a bridge symbol
		template<typename Cls>
		Obj* addClass()
		{
			// initialize new class bridge
			CppClass_Impl<Cls>* inst = new CppClass_Impl<Cls>();

			inst->classTemplate = new CppClassTemplate();

			Obj* obj = new Obj { Type::CPP_TYPE, Spec::SYMBOL };

			// downcast implementation into base class
			obj->_cppType = (CppClass*)inst;

			return obj;
		}		
		
		template<typename Cls, typename T>
		void addMember(VM* vm, str classSymbol, str memberSymbol, T Cls::* memberPtr)
		{
			// retrieve object with instance
			CppClass* inst = env::getSymbol(vm, classSymbol)->_cppType;

			// initialize new member bridge
			MemberBridge_Impl<Cls, T>* memberBridge = new MemberBridge_Impl<Cls, T>();

			// assign the member pointer
			memberBridge->member = memberPtr;

			memberBridge->index = inst->classTemplate->memberCount;
			++inst->classTemplate->memberCount;

			// set member objects to the default value of the instance members
			setTo<T>(inst->instMembers.emplace_back(new Obj{ Type::NIL, Spec::SYMBOL }), ((CppClass_Impl<Cls>*)inst)->inst.*memberPtr);

			inst->classTemplate->members.emplace(memberSymbol, memberBridge);
		}		
		
		template<typename Cls, typename Ret, typename... Args>
		void addMethod(VM* vm, str classSymbol, str methodSymbol, Ret(Cls::* method)(Args...))
		{
			// retrieve object with instance
			CppClass* inst = env::getSymbol(vm, classSymbol)->_cppType;

			// initialize new method bridge
			MethodBridge_Impl<Cls, Ret, Args...>* methodBridge = new MethodBridge_Impl<Cls, Ret, Args...>();

			// assign the method pointer
			methodBridge->method = method;

			inst->classTemplate->methods.emplace(methodSymbol, methodBridge);
		}		
	}
}

#endif 
