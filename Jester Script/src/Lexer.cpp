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


	void tokenizeFile(VM* vm, str src)
	{
		str::iterator ch = src.begin();

		str lexer = "";
		bool inLtrl = false;

		size_t line = 1;
		size_t depth = 0;

		/*
			User defined symbols may begin, end with, or contain 
			any ASCII symbol except '(' or ')' or ';', therefore all symbols must be
			separated by a space

			EX: 

			(++x) ;won't work with no space --> (++ x) ;works
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
			if (!lexer.empty()) addToken(vm, lexer, line);

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

					depth += (*ch == '(') - (*ch == ')');

					lexer += *ch;
					addToken(vm, lexer, line);
					break;

				case '\'':

					ch = addSpecialOp(vm, "quote", src, depth, line, ch);
					break;		
				
				case '~':

					ch = addSpecialOp(vm, "eval", src, depth, line, ch);
					break;
			}

			// break if reached end of file
			if (*ch == EOF) break;

			++ch; 
		}

		vm->tokenPtrCur = vm->tokenPtrBeg;
	}

	void addToken(VM* vm, str& symbol, size_t line)
	{
		vm->tokenPtrCur->symbol = symbol;
		vm->tokenPtrCur->line = line;
		matchTokenType(vm);

		// create and set the tokenPtr to the next item
		vm->tokenPtrCur->next = new Tok();
		vm->tokenPtrCur = vm->tokenPtrCur->next;

		symbol = "";
	}

	void matchTokenType(VM* vm)
	{
		const str& value = vm->tokenPtrCur->symbol;

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
		else if (env::getSymbol(vm, vm->tokenPtrCur->symbol))
		{
			vm->tokenPtrCur->spec = Spec::SYMBOL;
		}

		// if symbol is a literal value (5.0, 10, etc...)
		else if (tokIsLtrl(vm->tokenPtrCur))
		{
			vm->tokenPtrCur->spec = Spec::VALUE;
		}

		// if the symbol is not identified, mark as declared symbol
		else 
		{
			vm->tokenPtrCur->spec = Spec::SYMBOL;
		}
	}

	str::iterator addSpecialOp(VM* vm, str op, str& src, size_t& depth, int line, str::iterator cur)
	{
		str symbol = '(' + op + ' ';

		size_t totalOffset = cur - src.begin();
		src.replace(src.begin() + totalOffset, src.begin() + totalOffset + 1, symbol);
		cur = src.begin() + totalOffset;

		cur += symbol.length() - 1;

		int targetDepth = depth;

		while (true)
		{
			++cur;

			targetDepth += (*cur == '(') - (*cur == ')');

			if ((*cur == ' ' || *cur == ')' || *cur == EOF) && (targetDepth == depth || targetDepth == 0)) break;
		}

		src.insert(cur, ')');
		++depth;

		// initialize new node 

		vm->tokenPtrCur->symbol = '(';
		vm->tokenPtrCur->spec = Spec::BEG;

		vm->tokenPtrCur->next = new Tok();
		vm->tokenPtrCur = vm->tokenPtrCur->next;

		return src.begin() + totalOffset;
	}
}