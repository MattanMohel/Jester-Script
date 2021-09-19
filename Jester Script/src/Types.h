#ifndef TYPES_H
#define TYPES_H

#include <string>
#include <unordered_map>
#include <bitset>

#define ENUM_SIZE(e) (size_t)e::END

namespace jts
{
	using str = std::string;

	struct VM;
	struct Tok;
	struct Obj;
	struct Func;
	struct ObjNode;

	enum class Type
	{
		NIL = 0,

		CHAR,
		BOOL,
		INT,
		FLOAT,
		QUOTE,

		// needs attending to
		STRING,
	};

	enum class Spec
	{
		NIL = 0,

		SYMBOL,
		QUOTE,
		LTRL,
		FLAG,

		PARENTH_L,
		PARENTH_R,
	};

	enum class FnType
	{
		NIL = 0,

		NATIVE,
		C_BRIDGE,
		JTS,
	};

	enum class SFlag
	{
		NIL = 0,

		CONST,
		STATIC,
		REF, 

		END,
	};

	template<typename E, size_t S>
	struct Flag
	{
		std::bitset<S> bits = 0;

		bool Get(const E enumerator) const
		{
			return bits[(size_t)enumerator];
		}
		bool Set(const E enumerator, bool state = true)
		{
			bits[(size_t)enumerator] = state;
			return state;
		}

		void operator=  (Flag& flag)
		{
			bits = flag.bits;
		}

		void operator+= (Flag& flag)
		{
			bits |= flag.bits;
		}
	};
}

#endif
