#include "Types.h"
#include "Object.h"

namespace jts {

	template<> bool is<Var::CALLABLE_V>(Obj* obj) {
		return (size_t)obj->type >= (size_t)Type::CPP_FN && (size_t)obj->type <= (size_t)Type::JTS_FN;
	}
	
	template<> bool is<Var::CALLABLE_Q>(Obj* obj) {
		return obj->type == Type::QUOTE &&
			((size_t)obj->_quote->type >= (size_t)Type::CPP_FN && (size_t)obj->_quote->type <= (size_t)Type::JTS_FN);
	}

	template<> bool is<Var::INTEGRAL>(Obj* obj) {
		return (size_t)obj->type >= (size_t)Type::CPP_FN && (size_t)obj->type <= (size_t)Type::JTS_FN;
	}
}