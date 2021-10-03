#ifndef POOL_H
#define POOL_H

#define DEBUG_ALLOC 0

#include "../src/Types.h"
#include <iostream>
#include <queue>

using namespace jts;

template <typename T>
struct Node
{
	Node() = default;

	Node(T* value) : value(value)
	{}

	T* value = nullptr;
	Node<T>* next = nullptr;
	Node<T>* prev = nullptr;
};

template<typename T>
class Pool
{
public:

	using ResetFunc = T* (*)(T*);

	Pool(size_t size, ResetFunc resetFunc) : m_size(size), m_resetFunc(resetFunc)
	{
		while (size--)
		{
			m_buffer.push(new T());
		}
	}

	T* acquire()
	{
		if (!m_size)
		{

		#if DEBUG_ALLOC
			T* value = new T();
			std::cout << "allocating " << value << '\n';
			return value;
		#else
			return new T();
		#endif
		}

		--m_size;

	#if DEBUG_ALLOC
		std::cout << "acquiring " << m_buffer.front() << " - have " << m_size << '\n';
	#endif 

		T* value = m_buffer.front();
		m_buffer.pop();

		return m_resetFunc(value);
	}

	void release(T* value)
	{
		++m_size; 

	#if DEBUG_ALLOC
		std::cout << "releasing " << value << " - have " << m_size << '\n';
	#endif

		m_buffer.push(value);
	}

private:

	std::queue<T*> m_buffer;

	ResetFunc m_resetFunc;

	size_t m_size = 0;
};

#endif