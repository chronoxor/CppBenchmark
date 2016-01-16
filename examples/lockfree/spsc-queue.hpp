// This is free and unencumbered software released into the public domain.

// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software, either in source code form or as a compiled
// binary, for any purpose, commercial or non-commercial, and by any
// means.

// In jurisdictions that recognize copyright laws, the author or authors
// of this software dedicate any and all copyright interest in the
// software to the public domain. We make this dedication for the benefit
// of the public at large and to the detriment of our heirs and
// successors. We intend this dedication to be an overt act of
// relinquishment in perpetuity of all present and future rights to this
// software under copyright law.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.

// For more information, please refer to <http://unlicense.org/>


// non-intrusive lock free unbounded SPSC queue
// the algorithm was taken from the blog post below, and converted to C++11
// http://cbloomrants.blogspot.com/2009/02/02-26-09-low-level-threading-part-51.html

#ifndef __SPSC_QUEUE_INCLUDED__
#define __SPSC_QUEUE_INCLUDED__

#include <atomic>

template<typename T>
class spsc_queue_t
{
public:
    spsc_queue_t() : _head(new node_t), _tail(_head)
    {
        _head->next = NULL;
    }

    ~spsc_queue_t()
    {
        T output;
        while (this->dequeue(output)) {}
        delete _head;
    }

    void enqueue(const T& input)
    {
        node_t* node = new node_t;
        node->data = input;
        node->next = NULL;

        std::atomic_thread_fence(std::memory_order_acq_rel);
        _head->next = node;
        _head = node;
    }

    bool dequeue(T& output)
    {
        std::atomic_thread_fence(std::memory_order_consume);
        if (!_tail->next) {
            return false;
        }

        output = _tail->next->data;
        std::atomic_thread_fence(std::memory_order_acq_rel);
        _back = _tail;
        _tail = _back->next;

        delete _back;
        return true;
    }

private:
    struct node_t
    {
        node_t* next;
        T       data;
    };

    typedef char cache_line_pad_t[64];

    cache_line_pad_t _pad0;
    node_t*          _head;

    cache_line_pad_t _pad1;
    node_t*          _tail;
    node_t*          _back;

    spsc_queue_t(const spsc_queue_t&) {}
    void operator=(const spsc_queue_t&) {}
};

#endif
