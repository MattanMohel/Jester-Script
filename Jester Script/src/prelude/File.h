#ifndef FILE_H
#define FILE_H

#include "core/Types.h"
#include "core/Object.h"
#include "core/Execute.h"
#include "core/Log.h"
#include "core/VM.h"

#include "util/ObjectOp.h"
#include "util/Pool.h"

using namespace jts;

namespace lib {
	inline void FileLib(VM* vm) {

		// (repl)
		env::addSymbol(vm, "repl", env::addNative([](VM* vm, Node* args)
		{
			return env::runREPL(vm);
		}));

		// (compile path)
		env::addSymbol(vm, "compile", env::addNative([](VM* vm, Node* args)
		{
			str path = toString(evalObj(vm, args->val));

			Node* stackPtr = vm->stackPtrCur;
			env::addScript(vm, path, false, false);
			Obj* ret = env::run(vm);

			vm->stackPtrCur = stackPtr;

			return ret;
		}));

		// (pwd)
		env::addSymbol(vm, "pwd", env::addNative([](VM* vm, Node* args)
		{
			return setTo<str*>(env::newObj(vm), &vm->workingDir);
		}));
				
		// (cat file)
		env::addSymbol(vm, "cat", env::addNative([](VM* vm, Node* args)
		{
			return setTo<str*>(env::newObj(vm), new str(env::cat(vm, toString(evalObj(vm, args->val)))));
		}));

		// (cd path)
		env::addSymbol(vm, "cd", env::addNative([](VM* vm, Node* args)
		{
			env::cd(vm, toString(evalObj(vm, args->val)));
			return setTo<str*>(env::newObj(vm), &vm->workingDir);
		}));		
		
		// (ls)
		env::addSymbol(vm, "ls", env::addNative([](VM* vm, Node* args)
		{
			return setTo<str*>(env::newObj(vm), new str(env::ls(vm)));
		}));
	}
}

#endif