#ifndef STRCON_H
#define STRCON_H

#include "Types.h"

namespace jts
{
	// Checks if a token's string value is a literal

	bool tokIsLtrl(Tok* tok);

	// Converts a token literal to an object

	Obj* tokToLtrl(Tok* tok); 
}

#endif