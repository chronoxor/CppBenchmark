#ifndef __LOCK_BOUNDED_QUEUE_INCLUDED__
#define __LOCK_BOUNDED_QUEUE_INCLUDED__

#include <atomic>
#include <mutex>
#include <condition_variable>
#include <assert.h>

template<typename T>
class lock_bounded_queue_t
{
public:
    lock_bounded_queue_t(size_t size) : _size(size), _mask(size - 1), _buffer(new T[_size]), _head(0), _tail(0)
    {
        // make sure it's a power of 2
        assert((_size != 0) && ((_size & (~_size + 1)) == _size));
    }

    ~lock_bounded_queue_t()
    {
        delete[] _buffer;
    }

    void enqueue(T& input)
    {
		std::unique_lock<std::mutex> lock(_mtx);

		_cond_overflow.wait(lock, [this]() { return _tail + _size > _head; });
		_buffer[_head++ & _mask] = input;
		_cond_empty.notify_one();
    }

    void dequeue(T& output)
    {
		std::unique_lock<std::mutex> lock(_mtx);

		_cond_empty.wait(lock, [this]() { return _tail < _head;	});
		output = _buffer[_tail++ & _mask];
		_cond_overflow.notify_one();
    }

private:
    const size_t _size;
    const size_t _mask;
    T* const _buffer;

    size_t _head;
    size_t _tail;

    std::condition_variable _cond_empty;
    std::condition_variable _cond_overflow;
    std::mutex _mtx;

    lock_bounded_queue_t(const lock_bounded_queue_t&) {}
    void operator=(const lock_bounded_queue_t&) {}
};

#endif
