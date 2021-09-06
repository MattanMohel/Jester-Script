#include "Lexer.h"
#include "Token.h"
#include "StrCon.h"
#include "VM.h"

namespace jts
{
	void TokenizeFile(VM* vm, FILE* file)
	{
		char ch =  ' ';
		str lexer = "";

		size_t line = 1;

		/*
			User defined symbols may begin, end with, or contain 
			and ASCII symbol except '(' or ')' or '#', therefore all symbols must be
			separated by a space

			Ex: (++x) --> (++ x)
		*/

		vm->tokenPtrBeg = new Tok();
		vm->tokenPtrCur = vm->tokenPtrBeg;

		while (true)
		{
			// Extract a single token
			while (ch != '(' && ch != ')' && ch != '#' && ch != ' ' && ch != '\t' && ch != '\n' && ch != EOF)
			{
				lexer += ch;

				ch = fgetc(file);
			}

			if (!lexer.empty())
			{
				vm->tokenPtrCur->value = lexer;
				vm->tokenPtrCur->line  =  line;
				MatchTokenType(vm);

				vm->tokenPtrCur->rest = new Tok();
				vm->tokenPtrCur = vm->tokenPtrCur->rest;

				lexer.clear();
			}

			if (ch == '\n')
			{
				++line;
			}
			else if (ch == '#')
			{
				while (ch != '\n') { ch = fgetc(file); }
				++line;
			}
			else if (ch == '(' || ch == ')')
			{
				vm->tokenPtrCur->value = ch;
				vm->tokenPtrCur->line = line;
				MatchTokenType(vm);

				vm->tokenPtrCur->rest = new Tok();
				vm->tokenPtrCur = vm->tokenPtrCur->rest;
			}
			else if (ch == EOF)
			{
				break;
			}

			ch = fgetc(file);
		}

		vm->tokenPtrCur = vm->tokenPtrBeg;
	}

	void MatchTokenType(VM* vm)
	{
		const str& value = vm->tokenPtrCur->value;

		if (value.empty())
		{
			return;
		}

		else if (value == "(")
		{
			vm->tokenPtrCur->spec = Spec::PARENTH_L;
		}		
		
		else if (value == ")")
		{
			vm->tokenPtrCur->spec = Spec::PARENTH_R;
		}

		else if (vm->natives.find(value) != vm->natives.end())
		{
			vm->tokenPtrCur->fnType = FnType::NAT_FUNC;
			vm->tokenPtrCur->spec = Spec::FUNC;
		}

		else if (TokIsLtrl(vm->tokenPtrCur))
		{
			vm->tokenPtrCur->spec = Spec::LTRL;
		}

		else 
		{
			vm->tokenPtrCur->spec = Spec::SYMBOL;
		}
	}
}