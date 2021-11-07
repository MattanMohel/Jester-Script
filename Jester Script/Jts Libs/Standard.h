#ifndef STANDARD_H
#define STANDARD_H

#include "../src/VM.h"
#include "../src/Types.h"
#include "../src/Object.h"
#include "../src/Execute.h"
#include "../src/Operations.h"
#include "../src/JtsFunc.h"
#include "../src/JtsMacro.h"
#include "../src/JtsType.h"
#include "../src/CppClass.h"
#include "../src/Log.h"

#include <iostream>

using namespace jts;

namespace lib
{
	inline void StandardLib(VM* vm)
	{
		env::addSymbol(vm, "nil", env::addConst(nullptr));

		// (quote target)
		env::addSymbol(vm, "quote", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			return quoteObj(args, eval);
		}));

		// (set to-set value)
		env::addSymbol(vm, "set", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			return binaryOp<Binary::SET>(evalObj(args->value, eval), evalObj(args->next->value, eval));
		}));		
		
		// (typeof value)
		env::addSymbol(vm, "typeof", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			Obj* value = evalObj(args->value, eval);

			switch (value->type)
			{
				case Type::INT:

					return setTo(ret, new str("int"));
				
				case Type::FLOAT:

					return setTo(ret, new str("float"));
				
				case Type::CHAR:

					return setTo(ret, new str("char"));
				
				case Type::BOOL:

					return setTo(ret, new str("bool"));
				
				case Type::CPP_FN:
				case Type::NAT_FN:
				case Type::JTS_FN:

					return setTo(ret, new str("function"));
				
				case Type::MAC_FN:

					return setTo(ret, new str("macro"));

				case Type::LIST:

					return setTo(ret, new str("list"));

				case Type::JTS_TYPE:

					return setTo(ret, new str(value->_jtsType->typeName));

				default: // NIL

					return setTo(ret, new str("nil"));
			}
		}));		
		
		// (type members)
		env::addSymbol(vm, "type", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			ret->_jtsType = new JtsType();
			ret->type = Type::JTS_TYPE;

			auto member = args;

			while (member)
			{
				auto memberVal = evalObj(member->value, eval);
				auto memberCpy = env::glbl_objPool.acquire();

				binaryOp<Binary::SET>(memberCpy, memberVal);

				ret->_jtsType->members.emplace(memberVal->symbol, memberCpy);
				member = member->next;
			}

			return ret;
		}));		
		
		// (new value)
		env::addSymbol(vm, "new", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			auto value = evalObj(args->value, eval);
			ret->type = value->type;

			switch (value->type)
			{
				case Type::LIST:
				{
					ret->_args = env::acquireNode();

					auto elem = value->_args;
					auto list = ret->_args;

					binaryOp<Binary::SET>(list->value, elem->value);

					while (elem->next)
					{
						list->next = env::acquireNode();

						binaryOp<Binary::SET>(list->next->value, elem->next->value);

						elem = elem->next;
						list = list->next;
					}

					break;
				}		

				case Type::JTS_TYPE:
				{
					ret->_jtsType = new JtsType();

					for (auto& member : value->_jtsType->members)
					{
						auto* memberCpy = binaryOp<Binary::SET>(env::glbl_objPool.acquire(), member.second);

						ret->_jtsType->members.emplace(member.first, memberCpy);
					}

					break;
				}

				case Type::CPP_TYPE:
				{
					args->value->_cppType->instance(ret->_cppType);

					break;
				}

				default:

					binaryOp<Binary::SET>(ret, value);
			}

			return ret;
		}));		
		
		// (: type member) or (: type method args...)
		env::addSymbol(vm, ":", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			auto node = args;

			while (true)
			{
				switch (node->value->type)
				{
					case Type::JTS_TYPE:
					{
						auto member = node->value->_jtsType->members[node->next->value->symbol];

						if (!isCallable(member->type))
						{
							if (!node->next->next)
								return member;

							node = node->next;
							break;
						}

						auto prevNode = node->next;

						node->next = node->next->next;
						ret = member->_jtsFn->call(node, eval);

						node->next = prevNode;

						return ret;
					}

					case Type::CPP_TYPE:
					{
						if (node->value->_cppType->hasMember(node->next->value->symbol))
						{
							if (!node->next->next)
								return node->value->_cppType->getMember(node->next->value->symbol);

							node = node->next;
							break;
						}

						auto prevNode = node->next;

						node->next = node->next->next;
						ret = node->value->_cppType->call(node->next->value->symbol, ret, args);

						node->next = prevNode;

						return ret;
					}
				}
			}
		}));

		// (fn (params) body)
		env::addSymbol(vm, "fn", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			ret->spec = Spec::SYMBOL;
			ret->type = Type::JTS_FN;

			ret->_jtsFn = new JtsFn();
			ret->_jtsFn->codeBlock = args->next;

			auto param = args->value->_args;

			while (param)
			{
				param->value = evalObj(param->value);
				param = param->next;
			}

			ret->_jtsFn->params = args;

			return ret;
		}));

		// (macro (params) body)
		env::addSymbol(vm, "macro", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			ret->spec = Spec::SYMBOL;
			ret->type = Type::JTS_FN;

			ret->_macFn = new MacFn();
			ret->_macFn->codeBlock = args->next;

			auto param = args->value->_args;

			while (param)
			{
				param->value = evalObj(param->value);
				param = param->next;
			}

			ret->_macFn->params = args;

			return ret;
		}));					
		
		// (eval target)
		env::addSymbol(vm, "eval", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			return evalObj(evalObj(args->value, false), true);
		}));		
		
		// (loop cond body)
		env::addSymbol(vm, "loop", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			auto cond = args;
			auto block = args->next;

			bool state = isTrue(evalObj(cond->value, eval));

			while (state)
			{
				while (block->next)
				{
					evalObj(block->value, eval);
					block = block->next;
				}

				auto loopRet = evalObj(block->value, eval);

				state = isTrue(evalObj(cond->value, eval));

				if (!state) return loopRet;

				block = args->next;
			}
		}));	

		// (do body...)
		env::addSymbol(vm, "do", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			while (args->next)
			{
				evalObj(args->value, eval);
				args = args->next;
			}

			return evalObj(args->value, eval);
		}));		
		
		// (string-int value)
		env::addSymbol(vm, "string-int", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			return setTo(ret, std::stoi(*evalObj(args->value, eval)->_string));
		}));			
		
		// (string-bool value)
		env::addSymbol(vm, "string-bool", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			return setTo(ret, (bool)std::stoi(*evalObj(args->value, eval)->_string));
		}));			
		
		// (string-char value)
		env::addSymbol(vm, "string-char", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			return setTo(ret, (*evalObj(args->value, eval)->_string)[0]);
		}));		
		
		// (string-int value)
		env::addSymbol(vm, "string-float", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			return setTo(ret, std::stof(*evalObj(args->value, eval)->_string));
		}));			

		// (int value)
		env::addSymbol(vm, "int", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			return setTo(ret, castObj<j_int>(evalObj(args->value, eval)));
		}));			
		
		// (float value)
		env::addSymbol(vm, "float", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			return setTo(ret, castObj<j_float>(evalObj(args->value, eval)));
		}));			
		
		// (char value)
		env::addSymbol(vm, "char", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			return setTo(ret, castObj<j_char>(evalObj(args->value, eval)));
		}));			
		
		// (bool value)
		env::addSymbol(vm, "bool", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			return setTo(ret, castObj<j_bool>(evalObj(args->value, eval)));
		}));			
		
		// (string value)
		env::addSymbol(vm, "string", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			return setTo(ret, new str(toString(evalObj(args->value, eval))));
		}));			
		
		// (print args)
		env::addSymbol(vm, "print", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			while (args->next)
			{
				printObj(evalObj(args->value, eval), false);
				args = args->next;
			}

			return 	printObj(evalObj(args->value, eval), false);
		}));

		// (println args)
		env::addSymbol(vm, "println", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			while (args->next)
			{
				printObj(evalObj(args->value, eval), false);
				args = args->next;
			}

			return 	printObj(evalObj(args->value, eval), true);
		}));		
		
		// (input) --> (set in (input))
		env::addSymbol(vm, "input", env::addNative([](Obj* ret, ObjNode* args, bool eval) -> Obj*
		{
			str* input = new str();

			std::getline(std::cin, *input);

			return setTo(ret, input); 
		}));
	}
}

#endif