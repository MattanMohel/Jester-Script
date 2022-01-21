#include <stdlib.h>

#define REALLOC_FACTOR 1.5f

template<class T>
class Stack {

public:
    Stack() = default;

    T* pop() {
        T* popped = m_buffer[m_count - 1];
        m_buffer[m_count - 1] = nullptr;

        --m_count;

        return popped;
    }    
    
    T* peek() {
        return m_buffer[m_count - 1];
    }

    void setAt(size_t index, T value) {
        *m_buffer[index] = value;
    }

    T* getAt(size_t index) {
        return m_buffer[index];
    }

    void push(T* elm) {
        ++m_count;

        if (m_count > m_capacity) {
            reserve(m_capacity * REALLOC_FACTOR);
        }

        m_buffer[m_count - 1] = elm;
    }
     
    void reserve(size_t n) {
        m_capacity = n;

        m_buffer = (T**)realloc(m_buffer, sizeof(T*) * m_capacity);

        if (!m_buffer) {
            __debugbreak();
        }
    }

    void fill() {
        size_t count = m_capacity - m_count;

        void* alloc = malloc(sizeof(T) * count);

        for (size_t i = 0; i < count; ++i) {
            m_buffer[m_count + i] = (T*)alloc + sizeof(T) * i;
        }

        m_count = m_capacity;
    }

    bool empty() {
        return m_count = 0;
    }

    size_t capacity() const {
        return m_capacity;
    }

    size_t count() const {
        return m_count;
    }

private:
    size_t m_capacity = 0;
    size_t m_count    = 0;

    T** m_buffer;
};