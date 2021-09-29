#include "Pool.h"
#include "../src/Object.h"

ObjectPool::ObjectPool(size_t size)
{
	while (size--) { objectPool.push(new Obj); }
}

ObjectPool::~ObjectPool()
{
	while (!objectPool.empty())
	{
		delete objectPool.front();
		objectPool.pop();
	}
}

Obj* ObjectPool::pull()
{
	Obj* inst = objectPool.front();
	objectPool.pop(); 

	return inst;
}

void ObjectPool::push(Obj* rsc)
{
	rsc->_int = 0;
	rsc->type = Type::NIL;
	rsc->spec = Spec::NIL;
	rsc->symbol = "";

	objectPool.push(rsc);
}

size_t ObjectPool::size() const
{
	return objectPool.size();
}

bool ObjectPool::empty() const
{
	return objectPool.empty();
}