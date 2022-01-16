#ifndef CPP_CLASS_H
#define CPP_CLASS_H

#include "Types.h"
#include "Execute.h"
#include "VM.h"

#include "util/ObjectOp.h"

#define env_ADD_CLASS(vm, type) env::addSymbol(vm, #type, env::addClass<type>());
#define env_ADD_MEMBER(vm, type, member) env::addMember<type>(vm, #type, #member, &type::member);
#define env_ADD_METHOD(vm, type, method) env::addMethod<type>(vm, #type, #method, &type::method);

namespace jts {
	/*
	  A templated class used to port classes written in C++ to JTS

	  How It Works:

	  An instance class stores Object representations of its member variables
	   - All the C++ member pointers are stored within the ClassTemplate struct

	  Upon method invocation...
	  1) All object members are assigned back to the actual instance members
	  2) The methods used for invoking functions is repeated but with instance methods
	*/

	struct MemberBridge {
		// sets instance member to given object
		virtual void setMember(void* inst, Obj* obj) = 0;

		size_t index = 0;
	};

	template<typename Cls, typename T>
	struct MemberBridge_Impl : public MemberBridge {
		using Member = T Cls::*;

		void setMember(void* inst, Obj* obj) override {
			((Cls*)inst)->*member = castObj<T>(obj);
		}

		Member member;
	};

	struct MethodBridge {
		virtual Obj* call(VM* vm, void* inst, Obj* ret, Node* args) = 0;
	};

	template <typename Cls, typename Ret, typename... Args>
	struct MethodBridge_Impl : public MethodBridge {
		using Method = Ret(Cls::*)(Args...);

		Obj* call(VM* vm, void* inst, Obj* ret, Node* args) override {
			return call_Impl(vm, inst, ret, args, std::make_index_sequence<sizeof...(Args)>());
		}

		template<size_t... I>
		Obj* call_Impl(VM* vm, void* inst, Obj* ret, Node* args, std::index_sequence<I...>) {
			std::vector<Obj*> paramVec;

			// create parameter vector

			while (args->nxt) {
				paramVec.emplace_back(evalObj(vm, args->nxt->val));
				args = args->nxt;
			}

			if constexpr (std::is_void<Ret>::value)	// method is void, evaluate and return NIL
			{
				(((Cls*)inst)->*method)(castObj<Args>(paramVec[I])...);
				return setTo(ret, nullptr_t());
			}
			else // return the method value
			{
				return setTo(ret, (((Cls*)inst)->*method)(castObj<Args>(paramVec[I])...));
			}
		}

		Method method;
	};

	struct ClassTemplate {
		str typeName;
		std::unordered_map<str, MemberBridge*> members;
		std::unordered_map<str, MethodBridge*> methods;

		size_t memberCount = 0;
	};

	struct CppClass {
		// Invokes a method call
		virtual Obj* call(VM* vm, str& id, Obj* ret, Node* args) = 0;

		// creates a new class instance in JTS
		virtual void instance(VM* vm, CppClass*& cppClass) = 0;

		bool hasMember(str& symbol) {
			return classTemplate->members.find(symbol) != classTemplate->members.end();
		}

		Obj* getMember(str& symbol) {
			return instMembers[classTemplate->members[symbol]->index];
		}

		Obj* getMember_NonRef(str symbol) {
			return instMembers[classTemplate->members[symbol]->index];
		}

		ClassTemplate* classTemplate;

		std::vector<Obj*> instMembers;
	};

	template<typename Cls>
	struct CppClass_Impl : public CppClass {
		Obj* call(VM* vm, str& id, Obj* ret, Node* args) {
			for (auto& bridge : classTemplate->members) {
				bridge.second->setMember(&inst, instMembers[bridge.second->index]);
			}

			return classTemplate->methods[id]->call(vm, &inst, ret, args);
		}

		void instance(VM* vm, CppClass*& cppClass) override {
			CppClass_Impl<Cls>* newClass = new CppClass_Impl<Cls>();
			newClass->classTemplate = classTemplate;

			for (auto instMember : instMembers) {
				newClass->instMembers.emplace_back(setObj(vm, new Obj{ Type::NIL, Spec::SYMBOL }, instMember));
			}

			cppClass = (CppClass*)newClass;
		}

		Cls inst;
	};

	namespace env {
		
		// Add a bridge symbol
		template<typename Cls>
		Obj* addClass() {	
			// initialize new class bridge
			CppClass_Impl<Cls>* inst = new CppClass_Impl<Cls>();

			inst->classTemplate = new ClassTemplate();

			Obj* obj = new Obj{ Type::CPP_TYPE, Spec::SYMBOL };

			obj->_cppType = (CppClass*)inst;

			return obj;
		}

		template<typename Cls, typename T>
		void addMember(VM* vm, str classSymbol, str memberSymbol, T Cls::* memberPtr) {	
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
		void addMethod(VM* vm, str classSymbol, str methodSymbol, Ret(Cls::* method)(Args...)) {	
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
