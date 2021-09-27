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

	void TokenizeFile(VM* vm, str& src)
	{
		str::iterator ch = src.begin();

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
			while (inLtrl || (!IsPrefix(*ch) && *ch != ' ' && *ch != '\t' && *ch != '\n' && *ch != EOF))
			{
				if (*ch == '\"')
				{
					inLtrl = !inLtrl;
				}

				lexer += *ch;

				++ch;
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


			if (*ch == '\n')
			{
				++line;
			}
			else if (*ch == '#')
			{
				while (*ch != '\n') { ++ch; }
				++line;
			}
			else if (*ch == '(' || *ch == ')' || *ch == '\'')
			{
				vm->tokenPtrCur->value = *ch;
				vm->tokenPtrCur->line = line;
				MatchTokenType(vm);

				vm->tokenPtrCur->next = new Tok();
				vm->tokenPtrCur = vm->tokenPtrCur->next;
			}
			else if (*ch == EOF)
			{
				break;
			}

			++ch; 
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
			vm->tokenPtrCur->spec = Spec::HEAD;
		}		
		
		else if (value == ")")
		{
			vm->tokenPtrCur->spec = Spec::END;
		}		

		else if (env::GetSymbol(vm, vm->tokenPtrCur->value))
		{
			vm->tokenPtrCur->type = Type::NATIVE;

			vm->tokenPtrCur->spec = Spec::SYMBOL;
		}

		else if (TokIsLtrl(vm->tokenPtrCur))
		{
			vm->tokenPtrCur->spec = Spec::VALUE;
		}

		else 
		{
			vm->tokenPtrCur->spec = Spec::SYMBOL;
		}
	}

	str ExtractWord(str src)
	{
		src += EOF;

		str buffer = "";
		str::iterator it = src.begin();

		if (*it == ' ') ++it;

		while (*it != EOF && *it != ' ')
		{
			buffer += *it;
			++it;
		}

		return buffer;
	}
}