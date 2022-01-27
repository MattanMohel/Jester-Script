#ifndef TYPES_H
#define TYPES_H

#include <string>
#include <functional>
#include <filesystem>
#include <unordered_map>

namespace jts {
	using str = std::string;

	// JTS integral types

	using jtsi = int;
	using jtsb = bool;
	using jtsc = char;
	using jtsf = float;

	struct VM;
	struct Tok;
	struct Obj;
	struct JtsFn;
	struct MacFn;
	struct JtsType;
	struct CppFn;
	struct Node;

	struct CppClass;

	// Symbol type
	enum class Type {
		NIL = 0,

		CHAR,        // 1
		BOOL,        // 2
		INT,         // 3
		FLOAT,       // 4
				    
		QUOTE,       // 5
		LIST,        // 6
				    
		NAT_FN,      // 7
		CPP_FN,      // 8
		JTS_FN,      // 9

		JTS_TYPE,    // 11
		CPP_TYPE,    // 12
				     
		STRING,      // 13
	};

	// Symbol specialization
	enum class Spec {
		NIL = 0,

		SYMBOL, // variable
		VALUE,  // literal value

		LST_BEG, // '(' or '['
		LST_END, // ')' or ']'
	};

	// Symbol flags
	enum class SFlag {
		NIL = 0,

		STAT,
		CONST,
		MUT,

		END,
	};

	enum class State {
		NIL = 0,

		MES, // message
		WRN, // warning
		ERR, // error
	};

	inline bool isIntegral(Type type) {
		return (int)type >= (int)Type::CHAR && (size_t)type <= (int)Type::FLOAT;
	}
}

#endif
