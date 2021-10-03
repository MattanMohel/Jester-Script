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

	/*
		Remove and return back-most pool value
	*/
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

	/*
		Returns back-most pool value without removal
	*/
	T* peek()
	{
		if (m_buffer.empty())
		{
			return new T();
		}

		return m_buffer.back();
	}

	/*
		Insert value back into pool
	*/
	void release(T* value)
	{
		if (m_buffer.size() == m_buffer.capacity())
		{
			m_buffer.reserve(m_buffer.size() * 1.5);
		}

		m_buffer.emplace_back(value);
	}

	size_t size()
	{
		return m_buffer.size();
	}

private:

	std::vector<T*> m_buffer;
};

#endif