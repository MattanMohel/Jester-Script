#include "Object.h"
#include "Log.h"
#include "VM.h"

namespace jts {
	
	void Obj::assert(bool cond, str message) {
		env::assert(cond, mes(message, this->symbol));
	}
}