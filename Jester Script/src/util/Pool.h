#ifndef POOL_H
#define POOL_H

#include <functional>
#include <signal.h>

#include "core/Object.h"
using namespace jts;

#define ASSERT(cond, mes) if (cond) { printf(mes); raise(SIGABRT); }

#define DEBUG_POOL 1

template<typename T, int SZ>
class Pool_s {

    static_assert(SZ > 0, "pool size must be greater than 0");

    using CLR = std::function<T* (T*)>;

    struct N {
        N* nxt = nullptr;
        T val;
    };

public:
    void init(str id, CLR clear = [](T* val) { return val; }) {
        m_clear = new CLR(clear);
        m_debugID = id;
        m_index = 0;

        for (int i = 0; i < SZ - 1; ++i) {
            m_elements[i] = &m_buffer[i];
        }
    }

    T* acquire() {
        ASSERT(m_index == SZ - 1, "acquiring empty pool element");

        T* val = m_elements[m_index];
        m_elements[m_index] = nullptr;

        ++m_index;

    #if DEBUG_POOL
        printf("%s - acquiring - %p, have %d\n", m_debugID.c_str(), val, SZ - m_index);
    #endif

        return (*m_clear)(val);
    }

    void release(T* elm) { 
        ASSERT(is_elem(elm) >= SZ, "releasing foreign element");

        --m_index;

        m_elements[m_index] = elm;

    #if DEBUG_POOL
        printf("%s - releasing - %p, have %d\n", m_debugID.c_str(), elm, SZ - m_index);
    #endif
    }    

    // returns index of element in buffer
    long index_of(T* elm) const {
        return elm - &m_buffer[0];
    }

    // returns if element is in buffer
    bool is_elem(T* elm) const {
        long index = index_of(elm);

        return index > -1 && index < SZ;
    }

    size_t free() const {
        return m_index;
    }

    size_t size() const {
        return SZ;
    }

private:
    T    m_buffer[SZ];
    T* m_elements[SZ];

    CLR* m_clear;
    size_t m_index;

    str m_debugID;
};

#endif