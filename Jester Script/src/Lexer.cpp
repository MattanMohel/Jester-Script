#include "Lexer.h"
#include "Token.h"
#include "StrCon.h"
#include "VM.h"
#include <tuple>

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

	bool isSplicer(char value)
	{
		for (char pref : splicers)
		{
			if (value == pref) return true;
		}

		return false;
	}	
	
	bool isOperator(char value)
	{ 
		for (auto& pref : operators)
		{
			if (value == pref.first) return true;
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
			any ASCII symbol except for chars included in the 'prefixes' array (see LEXER.H), 
			all other symbols must be separated by a space

			EX: 

			(++x) won't work with no space --> (++ x) works
		*/

		// initialize Token linked-list head
		vm->tokenPtrBeg = new Tok();
		vm->tokenPtrCur = vm->tokenPtrBeg;

		while (true)
		{
			// extract a single token
			while (inLtrl || !(isPrefix(*ch) || isOperator(*ch) || isSplicer(*ch))) 
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

					while (*ch != '\n') { ++ch; } ++ch;
					++line;
					break;

				case '(':
				case ')':
				case '[':
				case ']':

					depth += (*ch == '(' || *ch == '[') - (*ch == ')' || *ch == ']');

					lexer += *ch;
					addToken(vm, lexer, line);
					break;

				case '\'':
				case '~':
				case ':':

					ch = addOperator(vm, src, depth, ch);
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
		else if (value == "(" || value == "[")
		{
			vm->tokenPtrCur->spec = Spec::LST_BEG;
		}				
		else if (value == ")" || value == "]")
		{
			vm->tokenPtrCur->spec = Spec::LST_END;
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
		else // a new symbol
		{ 
			vm->tokenPtrCur->spec = Spec::SYMBOL;
		}
	}

	str::iterator addOperator(VM* vm, str& src, size_t startDepth, str::iterator cur, bool head)
	{
		str symbol = '(' + std::get<str>(operators[*cur]) + ' ';
		if (head) symbol.insert(symbol.begin(), ' ');

		size_t elemCount = std::get<size_t>(operators[*cur]);

		// calculate offset to current iterator in src and insert symbol

		size_t totalOffset = cur - src.begin();
		src.replace(src.begin() + totalOffset, src.begin() + totalOffset + 1, symbol);
		cur = src.begin() + totalOffset;

		cur += symbol.length() - 1;

		int depth = startDepth;

		while (elemCount)
		{
			while (true)
			{
				++cur;

				if (isOperator(*cur)) cur = addOperator(vm, src, depth, cur, false);

				depth += (*cur == '(' || *cur == '[') - (*cur == ')' || *cur == ']');

				if (depth == startDepth && (isSplicer(*cur) || *cur == ')' || *cur == ']')) break;
			}

			--elemCount;
		}

		cur -= (*cur == EOF);

		src.insert(cur + 1, ')');
		++startDepth;

		return src.begin() + totalOffset;
	}
}