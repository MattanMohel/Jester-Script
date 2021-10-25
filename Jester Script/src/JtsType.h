#ifndef JTS_TYPE_H
#define JTS_TYPE_H

#include "Object.h"

namespace jts
{
	struct JtsType
	{
		std::unordered_map<str, Obj*> members;

		str typeName = "";
	};
}

#endif
