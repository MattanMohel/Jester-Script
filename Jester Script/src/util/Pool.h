#ifndef POOL_H
#define POOL_H

#include <functional>
#include <signal.h>

#include "core/Object.h"
using namespace jts;

#define ASSERT(cond, mes) if (cond) { printf(mes); raise(SIGABRT); }

#define DEBUG_POOL 0

template<typename T, int SZ>
class Pool_s {

    static_assert(SZ > 0, "pool size must be greater than 0");

    using CLR = std::function<T* (T*)>;

    struct N {
        T* nxt = nullptr;
        T  val;
    };

public:
    void init(CLR clear = [](T* val) { return val; }) {
        _clear = new CLR(clear);
        _free = SZ;

        _head = &_buffer[0].val;

        for (int i = 0; i < SZ - 1; ++i) {
            _buffer[i].nxt = &_buffer[i + 1].val;
        }
        _buffer[SZ - 1].nxt = nullptr;
    }

    T* acquire() {
        T* val = _head;

        size_t head_index = index_of(_head);

        ASSERT(!_head, "acquiring empty pool element");

    #if DEBUG_POOL
        printf("acquiring - %p\n", val);
    #endif

        --_free;

        _head = _buffer[head_index].nxt;

        return (*_clear)(val);
    }

    T* acquire(CLR clear) {
        T* val = _head;

        size_t head_index = index_of(_head);

        ASSERT(!_head, "acquiring empty pool element");

    #if DEBUG_POOL
        printf("acquiring - %p\n", val);
    #endif

        --_free;

        _head = _buffer[head_index].nxt;

        return clear(val);
    }

    T* peek() {
        return _head;
    }

    void release(T* elm) {
        size_t elm_index = index_of(elm);

        ASSERT(elm_index >= SZ, "releasing foreign element");

    #if DEBUG_POOL
        printf("releasing - %p\n", *elm);
    #endif

        ++_free;

        elm = nullptr;

        _buffer[elm_index].nxt = _head;
        _head = &_buffer[elm_index].val;
    }    
    
    void release(T** elm) {
        size_t elm_index = index_of(*elm);

        ASSERT(elm_index >= SZ, "releasing foreign element");

    #if DEBUG_POOL
        printf("releasing - %p\n", *elm);
    #endif

        ++_free;

        *elm = nullptr;

        _buffer[elm_index].nxt = _head;
        _head = &_buffer[elm_index].val;
    }

    // release Ts... elements back to buffer
    template<typename... Ts>
    void release_t(Ts... elms) {
        (release(&elms), ...);
    }

    void release_all() {
        _head = &_buffer[0].val;

        for (int i = 0; i < SZ - 1; ++i) {
            _buffer[i].nxt = &_buffer[i + 1].val;
        }
        _buffer[SZ - 1].nxt = nullptr;
    }

    // returns index of element in buffer
    size_t index_of(T* elm) {
        return (elm - &_buffer[0].val) / (sizeof(N) / sizeof(T));
    }

    // returns if element is in buffer
    bool elem_of(T* elm) {
        return index_of(elm) < SZ;
    }

    void set_clear(CLR clear) {
        _clear = &clear;
    }

    size_t free() {
        return _free;
    }

    size_t size() {
        return SZ;
    }

private:
    T* _head;
    N  _buffer[SZ];

    CLR* _clear;
    size_t _free;
};

#endif