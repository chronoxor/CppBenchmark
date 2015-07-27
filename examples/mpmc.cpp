//
// Created by Ivan Shynkarenka on 24.07.2015.
//

#include "macros.h"

#include <atomic>
#include <memory>

#include "cameron/blockingconcurrentqueue.h"
#include "cameron/concurrentqueue.h"
#include "folly/ProducerConsumerQueue.h"
#include "lockfree/mpmc-bounded-queue.hpp"

const int items_to_produce = 1000000;
const auto settings = CppBenchmark::Settings().Infinite().MPMCRange(1, 8, [](int producers_from, int producers_to, int& producers_result) { int r = producers_result; producers_result *= 2; return r; },
                                                                    1, 8, [](int consumers_from, int consumers_to, int& consumers_result) { int r = consumers_result; consumers_result *= 2; return r; });

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
        if ((_count > items_to_produce) && (_queue->try_enqueue(0)))
            context.StopProduce();

        if (_queue->try_enqueue(_count))
            ++_count;
    }

    void RunConsumer(CppBenchmark::ContextMPMC& context) override
    {
        int value;
        if (_queue->try_dequeue(value) && (value == 0))
            context.StopConsume();
    }

private:
    std::shared_ptr<TConcurrentQueue> _queue;
    std::atomic<int> _count;
};

class ProducerConsumerQueueBenchmark : public CppBenchmark::BenchmarkMPMC
{
public:
    using BenchmarkMPMC::BenchmarkMPMC;

protected:
    void Initialize(CppBenchmark::Context&) override
    {
        _queue = std::make_shared<folly::ProducerConsumerQueue<int>>(65536);
        _count = 1;
    }

    void Cleanup(CppBenchmark::Context&) override
    {
        _queue.reset();
    }

    void RunProducer(CppBenchmark::ContextMPMC& context) override
    {
        if ((_count > items_to_produce) && (_queue->write(0)))
            context.StopProduce();

        if (_queue->write(_count))
            ++_count;
    }

    void RunConsumer(CppBenchmark::ContextMPMC& context) override
    {
        int value;
        if (_queue->read(value) && (value == 0))
            context.StopConsume();
    }

private:
    std::shared_ptr<folly::ProducerConsumerQueue<int>> _queue;
    std::atomic<int> _count;
};

class MPMCBoundedQueueBenchmark : public CppBenchmark::BenchmarkMPMC
{
public:
    using BenchmarkMPMC::BenchmarkMPMC;

protected:
    void Initialize(CppBenchmark::Context&) override
    {
        _queue = std::make_shared<mpmc_bounded_queue_t<int>>(65536);
        _count = 1;
    }

    void Cleanup(CppBenchmark::Context&) override
    {
        _queue.reset();
    }

    void RunProducer(CppBenchmark::ContextMPMC& context) override
    {
        int value = 0;
        if ((_count > items_to_produce) && (_queue->enqueue(value)))
            context.StopProduce();

        value = _count;
        if (_queue->enqueue(value))
            ++_count;
    }

    void RunConsumer(CppBenchmark::ContextMPMC& context) override
    {
        int value;
        if (_queue->dequeue(value) && (value == 0))
            context.StopConsume();
    }

private:
    std::shared_ptr<mpmc_bounded_queue_t<int>> _queue;
    std::atomic<int> _count;
};

BENCHMARK_CLASS(ConcurrentQueueBenchmark<moodycamel::BlockingConcurrentQueue<int>>, "moodycamel::BlockingConcurrentQueue<int>", settings)
BENCHMARK_CLASS(ConcurrentQueueBenchmark<moodycamel::ConcurrentQueue<int>>, "moodycamel::ConcurrentQueueBenchmark<int>", settings)
//BENCHMARK_CLASS(ProducerConsumerQueueBenchmark, "folly::ProducerConsumerQueueBenchmark<int>", settings)
BENCHMARK_CLASS(MPMCBoundedQueueBenchmark, "mpmc_bounded_queue_t<int>", settings)

BENCHMARK_MAIN()
