#include "Lexer.h"
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

		// '#' will become part of a list of prefix operators --> '#' = comment | '\'' = quote | '`' = quasi-quote | '~' = unquote

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

				vm->tokenPtrCur->head = new Tok();
				vm->tokenPtrCur = vm->tokenPtrCur->head;

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

				vm->tokenPtrCur->head = new Tok();
				vm->tokenPtrCur = vm->tokenPtrCur->head;
			}
			else if (ch == EOF)
			{
				break;
			}

			//add token to vm
			ch = fgetc(file);

		}

		vm->tokenPtrEnd = vm->tokenPtrCur;
		vm->tokenPtrCur = vm->tokenPtrBeg;
	}
}

