#ifndef POOL_H
#define POOL_H

#include "../src/Types.h"

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

	Pool(size_t size) : m_size(size), m_listCur(&m_listBeg)
	{
		Node<T>* next = new Node<T>(new T());
		m_listCur->next = next;

		while (--size)
		{
			next->next = new Node<T>();
			next->prev = m_listCur;

			m_listCur = next;
			next = next->next;
		}

		next->prev = m_listCur;

		m_listCur = next;
	}

	Pool() : m_listCur(&m_listBeg)
	{}

	T* pull()
	{
		if (!m_size) return new T();

		Node<T>* node = m_listCur;
		T* value = node->value;

		m_listCur = m_listCur->prev;
		delete node;

		return value;
	}

	void push(T* value)
	{
		++m_size; 

		m_listCur->next = new Node<T>(value);
		m_listCur = m_listCur->next;
	}

private:

	Node<T> m_listBeg;

	Node<T>* m_listCur;
	size_t m_size = 0;
};

#endif