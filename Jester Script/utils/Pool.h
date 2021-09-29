#ifndef POOL_H
#define POOL_H

#include <queue>
#include "../src/Types.h"

using namespace jts;

class ObjectPool
{
public:
	ObjectPool(size_t size);

	~ObjectPool();

	Obj* pull();

	void push(Obj* rsc);

	size_t size() const;

	bool empty() const;

private:
	std::queue<Obj*> objectPool;
};

#endif