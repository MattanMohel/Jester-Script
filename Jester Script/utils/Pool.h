#ifndef POOL_H
#define POOL_H

#define DEBUG_ALLOC 0

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

		#if DEBUG_ALLOC
			std::cout << "allocating\n";
		#endif
			return new T();
		}

	#if DEBUG_ALLOC
		std::cout << "acquiring - " << m_buffer.size() << " left\n";
	#endif

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

		#if DEBUG_ALLOC
			std::cout << "reserving - " << m_buffer.size() * 1.5 << " bits\n";
		#endif

			m_buffer.reserve(m_buffer.size() * 1.5);
		}

	#if DEBUG_ALLOC
		std::cout << "releasing - " << m_buffer.size() << " left\n";
	#endif

		m_buffer.emplace_back(value);
	}

	size_t count()
	{
		return m_buffer.size();
	}

private:

	std::vector<T*> m_buffer;
};

#endif