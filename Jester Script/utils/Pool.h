#ifndef POOL_H
#define POOL_H

#define DEBUG_ALLOC 1

#include "../src/Types.h"
#include <iostream>
#include <vector>

using namespace jts;

template<typename T>
class Pool
{
public:

	Pool(size_t size)
	{
		m_buffer.reserve(size);

		while (m_buffer.size() < size)
		{
			m_buffer.emplace_back(new T());
		}
	}

	T* acquire()
	{
		if (m_buffer.empty())
		{
			return new T();
		}

		T* value = m_buffer.back();
		m_buffer.pop_back();

		return value;
	}

	void release(T* value)
	{
		if (m_buffer.size() == m_buffer.capacity())
		{
			m_buffer.reserve(m_buffer.size() * 1.5);
		}

		m_buffer.emplace_back(value);
	}

private:

	std::vector<T*> m_buffer;
};

#endif