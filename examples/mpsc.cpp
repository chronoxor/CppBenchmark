//
// Created by Ivan Shynkarenka on 24.07.2015.
//

#include "macros.h"

#include <atomic>
#include <memory>

#include "cameron/blockingconcurrentqueue.h"
#include "cameron/concurrentqueue.h"
#include "lockfree/mpmc-bounded-queue.hpp"
#include "lockfree/mpsc-queue.hpp"

const int queue_bound_size = 65536;
const int items_to_produce = 10000000;
const auto settings = CppBenchmark::Settings().Infinite().MPMCRange(1, 8, [](int producers_from, int producers_to, int& producers_result) { int r = producers_result; producers_result *= 2; return r; },
                                                                    1, 1, [](int consumers_from, int consumers_to, int& consumers_result) { return consumers_result++; });

template <class TConcurrentQueue>
class ConcurrentQueueBenchmark : public CppBenchmark::BenchmarkMPMC
{
public:
    using BenchmarkMPMC::BenchmarkMPMC;

protected:
    void Initialize(CppBenchmark::Context&) override
    {
        _queue = std::make_shared<TConcurrentQueue>();
        _count = 1;
    }

    void Cleanup(CppBenchmark::Context&) override
    {
        _queue.reset();
    }

    void RunProducer(CppBenchmark::ContextMPMC& context) override
    {
        if (_count >= items_to_produce) {
            if (_queue->try_enqueue(0))
                context.StopProduce();
            return;
        }

        if (_queue->try_enqueue(_count))
            ++_count;
    }

    void RunConsumer(CppBenchmark::ContextMPMC& context) override
    {
        int value = -1;
        if (_queue->try_dequeue(value) && (value == 0))
            context.StopConsume();
    }

private:
    std::shared_ptr<TConcurrentQueue> _queue;
    std::atomic<int> _count;
};

class MPMCBoundedQueueBenchmark : public CppBenchmark::BenchmarkMPMC
{
public:
    using BenchmarkMPMC::BenchmarkMPMC;

protected:
    void Initialize(CppBenchmark::Context&) override
    {
        _queue = std::make_shared<mpmc_bounded_queue_t<int>>(queue_bound_size);
        _count = 1;
    }

    void Cleanup(CppBenchmark::Context&) override
    {
        _queue.reset();
    }

    void RunProducer(CppBenchmark::ContextMPMC& context) override
    {
        if (_count >= items_to_produce) {
            int value = 0;
            if (_queue->enqueue(value))
                context.StopProduce();
            return;
        }

        int value = _count;
        if (_queue->enqueue(value))
            ++_count;
    }

    void RunConsumer(CppBenchmark::ContextMPMC& context) override
    {
        int value = -1;
        if (_queue->dequeue(value) && (value == 0))
            context.StopConsume();
    }

private:
    std::shared_ptr<mpmc_bounded_queue_t<int>> _queue;
    std::atomic<int> _count;
};

class MPSCQueueBenchmark : public CppBenchmark::BenchmarkMPMC
{
public:
    using BenchmarkMPMC::BenchmarkMPMC;

protected:
    void Initialize(CppBenchmark::Context&) override
    {
        _queue = std::make_shared<mpsc_queue_t<int>>();
        _count = 1;
    }

    void Cleanup(CppBenchmark::Context&) override
    {
        _queue.reset();
    }

    void RunProducer(CppBenchmark::ContextMPMC& context) override
    {
        if (_count >= items_to_produce) {
            _queue->enqueue(0);
            context.StopProduce();
            return;
        }

        _queue->enqueue(_count++);
    }

    void RunConsumer(CppBenchmark::ContextMPMC& context) override
    {
        int value = -1;
        if (_queue->dequeue(value) && (value == 0))
            context.StopConsume();
    }

private:
    std::shared_ptr<mpsc_queue_t<int>> _queue;
    std::atomic<int> _count;
};

BENCHMARK_CLASS(ConcurrentQueueBenchmark<moodycamel::BlockingConcurrentQueue<int>>, "moodycamel::BlockingConcurrentQueue<int>", settings)
BENCHMARK_CLASS(ConcurrentQueueBenchmark<moodycamel::ConcurrentQueue<int>>, "moodycamel::ConcurrentQueueBenchmark<int>", settings)
BENCHMARK_CLASS(MPMCBoundedQueueBenchmark, "lockfree::mpmc_bounded_queue_t<int>", settings)
BENCHMARK_CLASS(MPSCQueueBenchmark, "lockfree::mpsc_queue_t<int>", settings)

BENCHMARK_MAIN()
