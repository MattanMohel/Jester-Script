#ifndef VM_H
#define VM_H

struct Obj;
#include "Types.h"

namespace jts
{
	struct VM
	{
		Obj* stackPtrCur = nullptr;
		Obj* stackPtrBeg = nullptr;
		Obj* stackPtrEnd = nullptr;

		Tok* tokenPtrCur = nullptr;
		Tok* tokenPtrBeg = nullptr;
		Tok* tokenPtrEnd = nullptr;

		void Assert(bool cond, str assertion)
		{
			if (cond)
			{
				// print assertion and quit
			}
		}
	};
}

#endif

