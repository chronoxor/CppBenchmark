//
// Created by Ivan Shynkarenka on 24.07.2015.
//

#include "macros.h"

#include "cameron/readerwriterqueue.h"
#include "folly/ProducerConsumerQueue.h"
#include "lockfree/spsc-bounded-queue.hpp"
/*
class ReaderWriterQueueBenchmark : public CppBenchmark::BenchmarkMPMC
{
public:
    using BenchmarkMPMC::BenchmarkMPMC;

protected:
    void Initialize(Context&) override { _queue}
    void Cleanup(Context&) override {}

    void Run(CppBenchmark::Context& context) override
    {
        for (size_t i = 0; i < items.size(); ++i)
        {
            // Set the current item as minimal
            size_t min = i;

            // Found in rest items another minimal
            for (size_t j = i + 1; j < items.size(); ++j)
            {
                if (items[j] < items[min])
                    min = j;
            }

            // Swap the current item with minimal
            std::swap(items[i], items[min]);
        }
        context.metrics().AddItems(items.size());
    }

private:
    moodycamel::ReaderWriterQueue<int> _queue;
};
*/
BENCHMARK_MAIN()
