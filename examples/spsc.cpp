//
// Created by Ivan Shynkarenka on 24.07.2015.
//

#include "macros.h"

#include <atomic>

#include "cameron/readerwriterqueue.h"
#include "folly/ProducerConsumerQueue.h"
#include "lockfree/spsc-bounded-queue.hpp"

const int items_to_produce = 10000000;
const auto settings = CppBenchmark::Settings().Infinite().MPMC(1, 1);

template <class TReaderWriterQueue>
class ReaderWriterQueueBenchmark : public CppBenchmark::BenchmarkMPMC
{
public:
    using BenchmarkMPMC::BenchmarkMPMC;

protected:
    void Initialize(CppBenchmark::Context&) override { _count = 0; }

    void RunProducer(CppBenchmark::ContextMPMC& context) override
    {
        _queue.enqueue(++_count);

        if (_count == items_to_produce) {
            _queue.enqueue(0);
            context.StopProduce();
        }
    }

    void RunConsumer(CppBenchmark::ContextMPMC& context) override
    {
        int value;
        if (_queue.try_dequeue(value) && (value == 0))
            context.StopConsume();
    }

private:
    TReaderWriterQueue _queue;
    std::atomic<int> _count;
};

class ProducerConsumerQueueBenchmark : public CppBenchmark::BenchmarkMPMC
{
public:
    ProducerConsumerQueueBenchmark(const std::string& name, const CppBenchmark::SettingsMPMC& settings_mpmc = CppBenchmark::SettingsMPMC())
            : BenchmarkMPMC(name, settings_mpmc),
              _queue(65536)
    {}

protected:
    void Initialize(CppBenchmark::Context&) override { _count = 0; }

    void RunProducer(CppBenchmark::ContextMPMC& context) override
    {
        ++_count;
        while (!_queue.write(_count));

        if (_count == items_to_produce) {
            while (!_queue.write(0));
            context.StopProduce();
        }
    }

    void RunConsumer(CppBenchmark::ContextMPMC& context) override
    {
        int value;
        if (_queue.read(value) && (value == 0))
            context.StopConsume();
    }

private:
    folly::ProducerConsumerQueue<int> _queue;
    std::atomic<int> _count;
};

class SPSCBoundedQueueBenchmark : public CppBenchmark::BenchmarkMPMC
{
public:
    SPSCBoundedQueueBenchmark(const std::string& name, const CppBenchmark::SettingsMPMC& settings_mpmc = CppBenchmark::SettingsMPMC())
            : BenchmarkMPMC(name, settings_mpmc),
              _queue(65536)
    {}

protected:
    void Initialize(CppBenchmark::Context&) override { _count = 0; }

    void RunProducer(CppBenchmark::ContextMPMC& context) override
    {
        int value = ++_count;
        while (!_queue.enqueue(value));

        if (_count == items_to_produce) {
            value = 0;
            while (!_queue.enqueue(value));
            context.StopProduce();
        }
    }

    void RunConsumer(CppBenchmark::ContextMPMC& context) override
    {
        int value;
        if (_queue.dequeue(value) && (value == 0))
            context.StopConsume();
    }

private:
    spsc_bounded_queue_t<int> _queue;
    std::atomic<int> _count;
};

BENCHMARK_CLASS(ReaderWriterQueueBenchmark<moodycamel::ReaderWriterQueue<int>>, "moodycamel::ReaderWriterQueue<int>", settings)
BENCHMARK_CLASS(ReaderWriterQueueBenchmark<moodycamel::BlockingReaderWriterQueue<int>>, "moodycamel::BlockingReaderWriterQueue<int>", settings)
BENCHMARK_CLASS(ProducerConsumerQueueBenchmark, "folly::ProducerConsumerQueue<int>", settings)
BENCHMARK_CLASS(SPSCBoundedQueueBenchmark, "spsc_bounded_queue_t<int>", settings)

BENCHMARK_MAIN()
