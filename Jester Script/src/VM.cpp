#include "VM.h"
#include "Token.h"
#include "Execute.h"
#include "Object.h"
#include "Lexer.h"
#include "File.h"
#include "Log.h"

#include <iostream>

namespace jts { namespace env {

	extern Pool<Obj>     glbl_objPool(100);
	extern Pool<ObjNode> glbl_nodePool(100);

	void AddSymbol(VM* vm, str key, Obj* value)
	{
		vm->symbols.emplace(key, value);
	}

	Obj* AddNative(Obj* (*native)(Obj*, ObjNode*, bool))
	{
		Obj* obj = new Obj();
		
		obj->_native = native;
		obj->type = Type::NAT_FN;
		obj->spec = Spec::SYMBOL;

		return obj;
	}

	template<> Obj* AddConst(j_char value)
	{
		Obj* obj = new Obj { Type::CHAR, Spec::SYMBOL };
		obj->_char = value;

		return obj;
	}

	template<> Obj* AddConst(j_bool value)
	{
		Obj* obj = new Obj { Type::BOOL, Spec::SYMBOL };
		obj->_bool = value;

		return obj;
	}

	template<> Obj* AddConst(j_int  value)
	{
		Obj* obj = new Obj { Type::INT, Spec::SYMBOL };
		obj->_int = value;

		return obj;
	}

	template<> Obj* AddConst(j_float value)
	{
		Obj* obj = new Obj { Type::FLOAT, Spec::SYMBOL };
		obj->_float = value;

		return obj;
	}

	template<> Obj* AddConst(std::nullptr_t value)
	{
		Obj* obj = new Obj { Type::NIL, Spec::SYMBOL };

		return obj;
	}

	void AddLib(VM* vm, void(*lib)(VM* vm))
	{
		vm->libs.emplace_back(lib);

		lib(vm);
	}

	Obj* GetSymbol(VM* vm, str symbol)
	{
		if (vm->symbols.find(symbol) != vm->symbols.end()) 
		{
			return vm->symbols[symbol];
		}

		return nullptr;
	}

	void RunREPL(VM* vm)
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

			ParseSrc(vm, src);

			PrintObj(env::Run(vm), true);

		#if DEBUG_ALLOC
			std::cout << "have " << env::glbl_objPool.count() << " objects and " << env::glbl_nodePool.count() << " nodes\n";
		#endif
		}
	}

	Obj* Run(VM* vm)
	{
		vm->stackPtrCur = vm->stackPtrBeg;

		while (vm->stackPtrCur->next)
		{
			EvalObj(vm->stackPtrCur->value);

			vm->stackPtrCur = vm->stackPtrCur->next;
		}

		return EvalObj(vm->stackPtrCur->value);
	}
}}