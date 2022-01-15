#include "Lexer.h"
#include "Token.h"
#include "StrCon.h"
#include "VM.h"
#include <tuple>

namespace jts {

	bool isPrefix(char value) {
		for (char pref : prefixes) {
			if (value == pref) return true;
		}

		return false;
	}

	bool isSplicer(char value) {
		for (char splice : splicers) {
			if (value == splice) return true;
		}

		return false;
	}

	bool isOperator(char value) {
		for (auto& op : operators) {
			if (value == op) return true;
		}

		return false;
	}

	bool isNonToken(char value) {
		return isPrefix(value) || isSplicer(value) || isOperator(value);
	}

	void tokenizeFile(VM* vm, str src) {
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

		while (true) {
			// extract a single token
			while (inLtrl || !isNonToken(*ch)) {
				// if '\"', parse as string until next '\"'
				if (*ch == '\"') inLtrl = !inLtrl;

				lexer += *ch;

				++ch;
			}

			// add new Token if lexer isn't empty
			if (!lexer.empty()) addToken(vm, lexer, line);

			// ch here is either a prefix operators or empty
			switch (*ch) {
			case '\n':

				++line;
				break;

			case ';':

				while (*ch != '\n') { ++ch; } ++ch;
				++line;
				break;

			case '(':
			case ')':

				depth += (*ch == '(') - (*ch == ')');

				lexer += *ch;
				addToken(vm, lexer, line);
				break;

			case '\'':
				ch = addOperator(vm, src, "quote", ch);
				break;

			case '~':
				ch = addOperator(vm, src, "eval", ch);
				break;
			}

			// break if reached end of file
			if (*ch == EOF) break;

			++ch;
		}

		vm->tokenPtrCur = vm->tokenPtrBeg;
	}

	void addToken(VM* vm, str& symbol, size_t line) {
		vm->tokenPtrCur->symbol = symbol;
		vm->tokenPtrCur->line = line;
		matchTokenType(vm);

		// create and set the tokenPtr to the next item
		vm->tokenPtrCur->next = new Tok();
		vm->tokenPtrCur = vm->tokenPtrCur->next;

		symbol = "";
	}

	void matchTokenType(VM* vm) {
		const str& value = vm->tokenPtrCur->symbol;

		if (value.empty()) {
			return;
		}
		else if (value == "(") {
			vm->tokenPtrCur->spec = Spec::LST_BEG;
		}
		else if (value == ")") {
			vm->tokenPtrCur->spec = Spec::LST_END;
		}

		// if symbol is in the VM
		else if (env::getSymbol(vm, vm->tokenPtrCur->symbol)) {
			vm->tokenPtrCur->spec = Spec::SYMBOL;
		}

		// if symbol is a literal value (5.0, 10, etc...)
		else if (tokIsLtrl(vm->tokenPtrCur)) {
			vm->tokenPtrCur->spec = Spec::VALUE;
		}
		else // a new symbol
		{
			vm->tokenPtrCur->spec = Spec::SYMBOL;
		}
	}

	str::iterator addOperator(VM* vm, str& src, const str& rplc, str::iterator it) {
		// store offset to iterator
		size_t offset = it - src.begin();

		src.replace(it, it + 1, " (" + rplc + ' ');
		it = src.begin() + rplc.length() + offset + 2;

		int depth = 0;

		do {
			do {
				++it;
			} while (isOperator(*it));

			int prnth = (*it == '(') - (*it == ')');

			if (prnth != 0) {
				depth += prnth;
				++it;
			}
		} 
		while (!(depth == 0 && isNonToken(*it)));

		src.insert(it, ')');

		return src.begin() + offset;
	}
}