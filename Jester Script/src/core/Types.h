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
	struct NatFn;
	struct CppFn;
	struct JtsType;
	struct Node;
	struct MacFn;

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
				    
		CPP_FN,      // 7
		NAT_FN,      // 8
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

		LST_BEG, // '(' 
		LST_END, // ')' 
	};

	enum class Var {
		NIL = 0,

		CALLABLE_V,
		CALLABLE_Q,
		INTEGRAL,
	};

	template <Var Vt>
	inline bool is(Obj* obj) {
		static_assert(false, "Var type assertion unsupported");
	}

	template<> bool is<Var::CALLABLE_V>(Obj* obj);
	template<> bool is<Var::CALLABLE_Q>(Obj* obj);
	template<> bool is<Var::INTEGRAL>(Obj* obj);
}

#endif
