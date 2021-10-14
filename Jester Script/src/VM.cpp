#include "VM.h"
#include "Token.h"
#include "Execute.h"
#include "Object.h"
#include "Lexer.h"
#include "File.h"
#include "Log.h"

#include <iostream>

namespace jts { namespace env {

	extern Pool<Obj> glbl_objPool(100, [](Obj* value)
	{
		value->type = Type::NIL;
		value->_int = 0;

		return value;
	});

	extern Pool<ObjNode> glbl_nodePool(100, [](ObjNode* value)
	{
		value->next = nullptr;
		value->value = nullptr;

		return value;
	});

	void addSymbol(VM* vm, str key, Obj* value)
	{
		vm->symbols.emplace(key, value);
	}

	Obj* addNative(Obj* (*native)(Obj*, ObjNode*, bool))
	{
		Obj* obj = new Obj();
		
		obj->_native = native;
		obj->type = Type::NAT_FN;
		obj->spec = Spec::SYMBOL;

		return obj;
	}

	template<> Obj* addConst(j_char value)
	{
		Obj* obj = new Obj { Type::CHAR, Spec::SYMBOL };
		obj->_char = value;

		return obj;
	}

	template<> Obj* addConst(j_bool value)
	{
		Obj* obj = new Obj { Type::BOOL, Spec::SYMBOL };
		obj->_bool = value;

		return obj;
	}

	template<> Obj* addConst(j_int  value)
	{
		Obj* obj = new Obj { Type::INT, Spec::SYMBOL };
		obj->_int = value;

		return obj;
	}

	template<> Obj* addConst(j_float value)
	{
		Obj* obj = new Obj { Type::FLOAT, Spec::SYMBOL };
		obj->_float = value;

		return obj;
	}

	template<> Obj* addConst(std::nullptr_t value)
	{
		Obj* obj = new Obj { Type::NIL, Spec::SYMBOL };

		return obj;
	}

	void addLib(VM* vm, void(*lib)(VM* vm))
	{
		vm->libs.emplace_back(lib);

		lib(vm);
	}

	Obj* getSymbol(VM* vm, str symbol)
	{
		if (vm->symbols.find(symbol) != vm->symbols.end()) 
		{
			return vm->symbols[symbol];
		}

		return nullptr;
	}

	void runREPL(VM* vm)
	{
		str src;

		size_t inputCount = 0;

		while (1)
		{
			std::cout << "[" << inputCount++ << "]>> ";
			std::getline(std::cin, src);
			src += EOF;

			if (src.empty()) continue;

			// Reset VM

			vm->stackPtrBeg = vm->stackPtrCur = nullptr;
			vm->tokenPtrBeg = vm->tokenPtrCur = nullptr;

			// Run input

			parseSrc(vm, src);

			printObj(env::run(vm), true);

		#if DEBUG_ALLOC
			std::cout << "have " << env::glbl_objPool.count() << " objects and " << env::glbl_nodePool.count() << " nodes\n";
		#endif
		}
	}

	Obj* run(VM* vm)
	{
		vm->stackPtrCur = vm->stackPtrBeg;

		while (vm->stackPtrCur->next)
		{
			evalObj(vm->stackPtrCur->value);

			vm->stackPtrCur = vm->stackPtrCur->next;
		}

		return evalObj(vm->stackPtrCur->value);
	}
}}