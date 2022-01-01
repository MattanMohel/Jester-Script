#ifndef POOL_H
#define POOL_H

#include "../src/Types.h"
#include <iostream>
#include <vector>

using namespace jts;

template<typename T>
class Pool
{
	using InitFunc = T* (*)(T*);

public:

	Pool(str name, size_t size, InitFunc initFunc)
		: m_initFunc(initFunc), name(name)
	{
		m_buffer.reserve(size * 2);
		m_usedBuffer.reserve(size * 2);

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

		T* value = m_buffer.back();

	#if DEBUG_ALLOC
		std::cout << "acquiring " << value << " - " << m_buffer.size() << " " << name << "s left\n";
	#endif

		m_usedBuffer.emplace_back(value);
		m_buffer.pop_back();

		return m_initFunc(value);
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

		return m_initFunc(m_buffer.back());
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

			m_buffer.reserve(m_buffer.capacity() * 1.5);
			m_usedBuffer.reserve(m_buffer.capacity() * 1.5);
		}

	#if DEBUG_ALLOC
		std::cout << "releasing " << value << " - " << m_buffer.size() << " " << name << "s left\n";
	#endif

		m_buffer.emplace_back(value);
	}

	void release_all()
	{
		size_t size = m_usedBuffer.size();

	#if DEBUG_ALLOC
		std::cout << "releasing all (" << size << ") - " << m_buffer.size() << " left\n";
	#endif

		if (size > m_buffer.capacity())
		{
			m_buffer.reserve(size * 1.5);
		}

		while (size)
		{
			m_buffer.emplace_back(m_usedBuffer[size - 1]);
			m_usedBuffer.pop_back();

			--size;
		}
	}

	size_t count()
	{
		return m_buffer.size();
	}

private:

	std::vector<T*> m_buffer, m_usedBuffer;
	InitFunc m_initFunc;

	str name;
};

#endif