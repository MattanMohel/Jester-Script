#ifndef PARSER_H
#define PARSER_H

#include "Types.h"

namespace jts {
	// Convert a tokenized file into a linked list tree of objects for execution 

	void parseTokens(VM* vm);
}

#endif
