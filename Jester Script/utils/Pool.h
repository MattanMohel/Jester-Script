#ifndef POOL_H
#define POOL_H

#define DEBUG_ALLOC 1

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

	using ResetFunc = T* (*)(T* value);

public:

	Pool(size_t size, ResetFunc resetFunc) : m_size(size), m_resetFunc(resetFunc)
	{
		while (size--)
		{
			m_buffer.push(new T());
		}
	}

	Pool(ResetFunc resetFunc) : m_resetFunc(resetFunc)
	{}

	T* acquire()
	{
		if (!m_size)
		{

		#if DEBUG_ALLOC
			std::cout << "allocating\n"; 
		#endif

			return new T();
		}

		--m_size;

	#if DEBUG_ALLOC
		std::cout << "acquiring - have " << m_size << '\n';
	#endif 

		T* value = m_resetFunc(m_buffer.front());
		m_buffer.pop();

		return value;
	}

	void release(T* value)
	{
		++m_size; 

	#if DEBUG_ALLOC
		std::cout << "releasing - have " << m_size << '\n';
	#endif

		m_buffer.push(value);
	}

private:

	std::queue<T*> m_buffer;

	size_t m_size = 0;

	ResetFunc m_resetFunc;
};

#endif