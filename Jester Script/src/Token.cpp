#include "Token.h"

namespace jts
{
	Tok* IncrTok(Tok*& tok, size_t index)
	{
		for (size_t i = 0; i < index; ++i)
		{
			tok = tok->rest;
		}

		return tok;
	}

	Tok* SeekTok(Tok* tok, size_t index)
	{
		for (size_t i = 0; i < index; ++i)
		{
			tok = tok->rest;
		}

		return tok;
	}
}