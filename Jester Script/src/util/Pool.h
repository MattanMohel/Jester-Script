#ifndef POOL_H
#define POOL_H

#include "core/Types.h"
#include "core/VM.h"

#include <iostream>
#include <vector>

#define DEBUG_ALLOC 0

using namespace jts;

template<typename T>
class Pool {
	using InitFunc = T * (*)(T*);

public:

	Pool(str name, size_t size, InitFunc init)
		: m_init(init), name(name) {
		
		m_buffer.reserve(size * 2);
		m_used.reserve(size * 2);

		while (m_buffer.size() < size) {
			m_buffer.emplace_back(new T());
		}
	}

	// Remove and return back-most pool value

	T* acquire() {
 		if (m_buffer.empty()) {

		#if DEBUG_ALLOC
			std::cout << "allocating\n";
		#endif
			return m_init(new T());
		}

		T* value = m_buffer.back();

	#if DEBUG_ALLOC
		std::cout << "acquiring " << value << " - " << m_buffer.size() << " " << name << "s left\n";
	#endif

		m_buffer.pop_back();

		return m_init(value);
	}

	
	// Return back-most pool value without removal
	
	T* peek() {
		if (m_buffer.empty()) {
			return new T();
		}

		return m_init(m_buffer.back());
	}

	
	// Insert value back into pool
	
	void release(T* value) {
		if (m_buffer.size() == m_buffer.capacity()) {

		#if DEBUG_ALLOC
			std::cout << "reserving buffer - " << m_buffer.size() * 1.5 << " bits\n";
		#endif

			m_buffer.reserve(m_buffer.capacity() * 1.5);
		}

	#if DEBUG_ALLOC
		std::cout << "releasing " << value << " - " << m_buffer.size() << " " << name << "s left\n";
	#endif

		m_buffer.emplace_back(value);
	}

	// push a used value to used vec

	void push_used(T* value) {
		if (m_used.size() == m_used.capacity()) {

		#if DEBUG_ALLOC
			std::cout << "reserving used - " << m_buffer.size() * 1.5 << " bits\n";
		#endif

			m_used.reserve(m_used.capacity() * 1.5);
		}

		m_used.emplace_back(value);
	}

	// release all used values

	void release_used() {
		m_buffer.insert(m_buffer.end(), m_used.begin(), m_used.end());
		m_used.clear();
	}

	size_t count() {
		return m_buffer.size();
	}

private:

	std::vector<T*> m_buffer;
	std::vector<T*> m_used;

	InitFunc m_init;

	str name;
};

#endif