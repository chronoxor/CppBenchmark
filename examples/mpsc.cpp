//
// Created by Ivan Shynkarenka on 24.07.2015
//

#include "benchmark/cppbenchmark.h"

#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow" // GCC: warning: declaration of 'expression' shadows a parameter or member
#elif defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable: 4127) // C4127: conditional expression is constant
#pragma warning(disable: 4702) // C4702: unreachable code
#endif
#include "cameron/blockingconcurrentqueue.h"
#include "cameron/concurrentqueue.h"
#include "lockfree/lock-bounded-queue.hpp"
#include "lockfree/mpmc-bounded-queue.hpp"
#include "lockfree/mpsc-queue.hpp"
#if defined(__GNUC__)
#pragma GCC diagnostic pop
#elif defined(_MSC_VER)
#pragma warning(pop)
#endif

#include <atomic>
#include <memory>

const int queue_bound_size = 65536;
const int items_to_produce = 10000000;
const auto settings = CppBenchmark::Settings().Infinite().PCRange(1, 8, [](int producers_from, int producers_to, int& producers_result) { int r = producers_result; producers_result *= 2; return r; },
                                                                  1, 1, [](int consumers_from, int consumers_to, int& consumers_result) { return consumers_result++; });

template <class TConcurrentQueue>
class ConcurrentQueueBenchmark : public CppBenchmark::BenchmarkPC
{
public:
    using BenchmarkPC::BenchmarkPC;

protected:
    void Initialize(CppBenchmark::ContextPC& context) override
    {
        _queue = std::make_shared<TConcurrentQueue>();
        _count = 1;
    }

    void Cleanup(CppBenchmark::ContextPC& context) override
    {
        _queue.reset();
    }

    void RunProducer(CppBenchmark::ContextPC& context) override
    {
        if (_count >= items_to_produce)
        {
            if (_queue->try_enqueue(0))
                context.StopProduce();
            return;
        }

        if (_queue->try_enqueue(_count))
            ++_count;
    }

    void RunConsumer(CppBenchmark::ContextPC& context) override
    {
        int value = -1;
        if (_queue->try_dequeue(value) && (value == 0))
            context.StopConsume();
    }

private:
    std::shared_ptr<TConcurrentQueue> _queue;
    std::atomic<int> _count;
};

class MPSCQueueBenchmark : public CppBenchmark::BenchmarkPC
{
public:
    using BenchmarkPC::BenchmarkPC;

protected:
    void Initialize(CppBenchmark::ContextPC& context) override
    {
        _queue = std::make_shared<mpsc_queue_t<int>>();
        _count = 1;
    }

    void Cleanup(CppBenchmark::ContextPC& context) override
    {
        _queue.reset();
    }

    void RunProducer(CppBenchmark::ContextPC& context) override
    {
        if (_count >= items_to_produce)
        {
            _queue->enqueue(0);
            context.StopProduce();
            return;
        }

        _queue->enqueue(_count++);
    }

    void RunConsumer(CppBenchmark::ContextPC& context) override
    {
        int value = -1;
        if (_queue->dequeue(value) && (value == 0))
            context.StopConsume();
    }

private:
    std::shared_ptr<mpsc_queue_t<int>> _queue;
    std::atomic<int> _count;
};

class MPMCBoundedQueueBenchmark : public CppBenchmark::BenchmarkPC
{
public:
    using BenchmarkPC::BenchmarkPC;

protected:
    void Initialize(CppBenchmark::ContextPC& context) override
    {
        _queue = std::make_shared<mpmc_bounded_queue_t<int>>(queue_bound_size);
        _count = 1;
    }

    void Cleanup(CppBenchmark::ContextPC& context) override
    {
        _queue.reset();
    }

    void RunProducer(CppBenchmark::ContextPC& context) override
    {
        if (_count >= items_to_produce)
        {
            int value = 0;
            if (_queue->enqueue(value))
                context.StopProduce();
            return;
        }

        int value = _count;
        if (_queue->enqueue(value))
            ++_count;
    }

    void RunConsumer(CppBenchmark::ContextPC& context) override
    {
        int value = -1;
        if (_queue->dequeue(value) && (value == 0))
            context.StopConsume();
    }

private:
    std::shared_ptr<mpmc_bounded_queue_t<int>> _queue;
    std::atomic<int> _count;
};

class LockBoundedQueueBenchmark : public CppBenchmark::BenchmarkPC
{
public:
    using BenchmarkPC::BenchmarkPC;

protected:
    void Initialize(CppBenchmark::ContextPC& context) override
    {
        _queue = std::make_shared<lock_bounded_queue_t<int>>(queue_bound_size);
        _count = 1;
    }

    void Cleanup(CppBenchmark::ContextPC& context) override
    {
        _queue.reset();
    }

    void RunProducer(CppBenchmark::ContextPC& context) override
    {
        if (_count >= items_to_produce)
        {
            int value = 0;
            _queue->enqueue(value);
            context.StopProduce();
            return;
        }

        int value = _count;
        _queue->enqueue(value);
        ++_count;
    }

    void RunConsumer(CppBenchmark::ContextPC& context) override
    {
        int value = -1;
        _queue->dequeue(value);
        if (value == 0)
            context.StopConsume();
    }

private:
    std::shared_ptr<lock_bounded_queue_t<int>> _queue;
    std::atomic<int> _count;
};

BENCHMARK_CLASS(ConcurrentQueueBenchmark<moodycamel::BlockingConcurrentQueue<int>>, "moodycamel::BlockingConcurrentQueue<int>", settings)
BENCHMARK_CLASS(ConcurrentQueueBenchmark<moodycamel::ConcurrentQueue<int>>, "moodycamel::ConcurrentQueueBenchmark<int>", settings)
BENCHMARK_CLASS(MPSCQueueBenchmark, "lockfree::mpsc_queue_t<int>", settings)
BENCHMARK_CLASS(MPMCBoundedQueueBenchmark, "lockfree::mpmc_bounded_queue_t<int>", settings)
BENCHMARK_CLASS(LockBoundedQueueBenchmark, "lockfree::lock_bounded_queue_t<int>", settings)

BENCHMARK_MAIN()
