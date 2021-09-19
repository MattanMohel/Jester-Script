#include "Lexer.h"
#include "Token.h"
#include "StrCon.h"
#include "VM.h"

namespace jts
{
	bool IsPrefix(char value)
	{
		for (char pref : prefixes)
		{
			if (value == pref)
			{
				return true;
			}
		}

		return false;
	}

	void TokenizeFile(VM* vm, FILE* file)
	{
		char ch =  ' ';
		str lexer = "";
		bool inLtrl = false;

		size_t line = 1;

		/*
			User defined symbols may begin, end with, or contain 
			and ASCII symbol except '(' or ')' or '#', therefore all symbols must be
			separated by a space

			EX: 

			(++x) --> (++ x)
		*/

		vm->tokenPtrBeg = new Tok();
		vm->tokenPtrCur = vm->tokenPtrBeg;

		while (true)
		{
			// Extract a single token
			while (inLtrl || (!IsPrefix(ch) && ch != ' ' && ch != '\t' && ch != '\n' && ch != EOF))
			{
				if (ch == '\'' || ch == '\"')
				{
					inLtrl = !inLtrl;
				}

				lexer += ch;

				ch = fgetc(file);
			}

			if (!lexer.empty())
			{
				vm->tokenPtrCur->value = lexer;
				vm->tokenPtrCur->line  =  line;
				MatchTokenType(vm);

				vm->tokenPtrCur->next = new Tok();
				vm->tokenPtrCur = vm->tokenPtrCur->next;

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

				vm->tokenPtrCur->next = new Tok();
				vm->tokenPtrCur = vm->tokenPtrCur->next;
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
		static Tok* prevTok = nullptr;

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

		else if (value == "const")
		{
			vm->tokenPtrCur->spec = Spec::FLAG;
			vm->tokenPtrCur->flag.Set(SFlag::CONST);
		}		

		else if (env::GetSymbol(vm, vm->tokenPtrCur))
		{
			vm->tokenPtrCur->fnType = FnType::NATIVE;

			if (prevTok->spec == Spec::PARENTH_L)
			{
				vm->tokenPtrCur->spec = Spec::CALL;
			}
			else
			{
				vm->tokenPtrCur->spec = Spec::SYMBOL;
			}
		}

		else if (TokIsLtrl(vm->tokenPtrCur))
		{
			vm->tokenPtrCur->spec = Spec::VALUE;
		}

		else 
		{
			vm->tokenPtrCur->spec = Spec::SYMBOL;
		}

		prevTok = vm->tokenPtrCur;
	}
}