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
#include "cameron/readerwriterqueue.h"
#include "folly/ProducerConsumerQueue.h"
#include "lockfree/lock-bounded-queue.hpp"
#include "lockfree/spsc-queue.hpp"
#include "lockfree/spsc-bounded-queue.hpp"
#if defined(__GNUC__)
#pragma GCC diagnostic pop
#elif defined(_MSC_VER)
#pragma warning(pop)
#endif

#include <atomic>
#include <memory>

const int queue_bound_size = 65536;
const int items_to_produce = 10000000;
const auto settings = CppBenchmark::Settings().Infinite().PC(1, 1);

template <class TReaderWriterQueue>
class ReaderWriterQueueBenchmark : public CppBenchmark::BenchmarkPC
{
public:
    using BenchmarkPC::BenchmarkPC;

protected:
    void Initialize(CppBenchmark::ContextPC& context) override
    {
        _queue = std::make_shared<TReaderWriterQueue>();
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
    std::shared_ptr<TReaderWriterQueue> _queue;
    std::atomic<int> _count;
};

class ProducerConsumerQueueBenchmark : public CppBenchmark::BenchmarkPC
{
public:
    using BenchmarkPC::BenchmarkPC;

protected:
    void Initialize(CppBenchmark::ContextPC& context) override
    {
        _queue = std::make_shared<folly::ProducerConsumerQueue<int>>(queue_bound_size);
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
            if (_queue->write(0))
                context.StopProduce();
            return;
        }

        if (_queue->write(_count))
            ++_count;
    }

    void RunConsumer(CppBenchmark::ContextPC& context) override
    {
        int value = -1;
        if (_queue->read(value) && (value == 0))
            context.StopConsume();
    }

private:
    std::shared_ptr<folly::ProducerConsumerQueue<int>> _queue;
    std::atomic<int> _count;
};

class SPSCQueueBenchmark : public CppBenchmark::BenchmarkPC
{
public:
    using BenchmarkPC::BenchmarkPC;

protected:
    void Initialize(CppBenchmark::ContextPC& context) override
    {
        _queue = std::make_shared<spsc_queue_t<int>>();
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
    std::shared_ptr<spsc_queue_t<int>> _queue;
    std::atomic<int> _count;
};

class SPSCBoundedQueueBenchmark : public CppBenchmark::BenchmarkPC
{
public:
    using BenchmarkPC::BenchmarkPC;

protected:
    void Initialize(CppBenchmark::ContextPC& context) override
    {
        _queue = std::make_shared<spsc_bounded_queue_t<int>>(queue_bound_size);
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
    std::shared_ptr<spsc_bounded_queue_t<int>> _queue;
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

BENCHMARK_CLASS(ReaderWriterQueueBenchmark<moodycamel::BlockingReaderWriterQueue<int>>, "moodycamel::BlockingReaderWriterQueue<int>", settings)
BENCHMARK_CLASS(ReaderWriterQueueBenchmark<moodycamel::ReaderWriterQueue<int>>, "moodycamel::ReaderWriterQueue<int>", settings)
BENCHMARK_CLASS(ProducerConsumerQueueBenchmark, "folly::ProducerConsumerQueue<int>", settings)
BENCHMARK_CLASS(SPSCQueueBenchmark, "lockfree::spsc_queue_t<int>", settings)
BENCHMARK_CLASS(SPSCBoundedQueueBenchmark, "lockfree::spsc_bounded_queue_t<int>", settings)
BENCHMARK_CLASS(LockBoundedQueueBenchmark, "lockfree::lock_bounded_queue_t<int>", settings)

BENCHMARK_MAIN()
