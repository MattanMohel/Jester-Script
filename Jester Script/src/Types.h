#ifndef TYPES_H
#define TYPES_H

#include <string>
#include <unordered_map>
#include <functional>

#include "../utils/IterableStack.h"

namespace jts {
	using str = std::string;

	// JTS integral types

	using j_int = int;
	using j_bool = bool;
	using j_char = char;
	using j_float = float;

	struct VM;
	struct Tok;
	struct Obj;
	struct JtsFn;
	struct MacFn;
	struct JtsType;
	struct CppFn;
	struct ObjNode;

	struct CppClass;

	// Symbol type
	enum class Type {
		NIL = 0,

		CHAR,      // 1
		BOOL,      // 2
		INT,       // 3
		FLOAT,     // 4

		QUOTE,     // 5
		LIST,      // 6

		NAT_FN,    // 7
		CPP_FN,    // 8
		JTS_FN,    // 9

		JTS_TYPE,  // 10
		CPP_TYPE,  // 11

		STRING,    // 12
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

	struct SymbolMap {
		std::unordered_map<str, Obj*> symbols;

		SymbolMap *next, *prev;

		size_t prnth_depth;
		bool open = false;
	};

	inline bool isIntegral(Type type) {
		return (size_t)type > 0 && (size_t)type < 5;
	}

	inline bool isValue(Type type) {
		return ((size_t)type > 0 && (size_t)type < 5) || ((size_t)type > 9 && (size_t)type < 13);
	}

	inline bool isCallable(Type type) {
		return (size_t)type > 6 && (size_t)type < 11;
	}
}

#endif
