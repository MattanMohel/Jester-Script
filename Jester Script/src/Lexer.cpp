#include "Lexer.h"
#include "Token.h"
#include "StrCon.h"
#include "VM.h"

namespace jts
{
	bool isPrefix(char value)
	{
		for (char pref : prefixes)
		{
			if (value == pref) return true;
		}

		return false;
	}


	void TokenizeFile(VM* vm, str src)
	{
		str::iterator ch = src.begin();

		str lexer = "";
		bool inLtrl = false;

		size_t line = 1;

		/*
			User defined symbols may begin, end with, or contain 
			any ASCII symbol except '(' or ')' or ';', therefore all symbols must be
			separated by a space

			EX: 

			(++x) ;won't work with nos space --> (++ x) ;works
		*/

		// initialize Token linked-list head
		vm->tokenPtrBeg = new Tok();
		vm->tokenPtrCur = vm->tokenPtrBeg;

		while (true)
		{
			// extract a single token
			while (inLtrl || (!isPrefix(*ch) && *ch != ' ' && *ch != '\t' && *ch != '\n' && *ch != EOF))
			{
				// if '\"', parse as string until next '\"'
				if (*ch == '\"') inLtrl = !inLtrl;

				lexer += *ch;

				++ch;
			}

			// add new Token if lexer isn't empty
			if (!lexer.empty()) AddToken(vm, lexer, line);

			// ch here is either a prefix operators or empty
			switch (*ch)
			{
				case '\n':

					++line;
					break;

				case ';':

					while (*ch != '\n') { ++ch; }
					++line;
					break;

				case '(':
				case ')':

					lexer += *ch;
					AddToken(vm, lexer, line);
					break;
			}

			// break if reached end of file
			if (*ch == EOF) break;

			++ch; 
		}

		vm->tokenPtrCur = vm->tokenPtrBeg;
	}

	void AddToken(VM* vm, str& symbol, size_t line)
	{
		vm->tokenPtrCur->value = symbol;
		vm->tokenPtrCur->line = line;
		MatchTokenType(vm);

		// create and set the tokenPtr to the next item
		vm->tokenPtrCur->next = new Tok();
		vm->tokenPtrCur = vm->tokenPtrCur->next;

		symbol = "";
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
			vm->tokenPtrCur->spec = Spec::BEG;
		}		
		
		else if (value == ")")
		{
			vm->tokenPtrCur->spec = Spec::END;
		}

		// if symbol is in the VM
		else if (env::GetSymbol(vm, vm->tokenPtrCur->value))
		{
			vm->tokenPtrCur->spec = Spec::SYMBOL;
		}

		// if symbol is a literal value (5.0, 10, etc...)
		else if (TokIsLtrl(vm->tokenPtrCur))
		{
			vm->tokenPtrCur->spec = Spec::VALUE;
		}

		// if the symbol is not identified, mark as declared symbol
		else 
		{
			vm->tokenPtrCur->spec = Spec::SYMBOL;
		}
	}
}