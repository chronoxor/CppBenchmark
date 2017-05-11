//
// Created by Ivan Shynkarenka on 11.05.2017
//

#include "benchmark/cppbenchmark.h"

#include <cstring>
#include <vector>

const uint64_t iterations = 1000000;
const int chunk_size_from = 1024;
const int chunk_size_to = 65536;
const int chunks = 100;
const auto settings = CppBenchmark::Settings().Iterations(iterations).ParamRange(chunk_size_from, chunk_size_to, [](int from, int to, int& result) { int r = result; result *= 2; return r; });

class MemoryCopyFixture
{
protected:
    std::vector<uint8_t*> buffers;

    MemoryCopyFixture()
    {
        // Create memory chunks
        for (int i = 0; i < chunks; ++i)
        {
            uint8_t* buffer = (uint8_t*)malloc(chunk_size_to);
            buffers.push_back(buffer);
        }
    }

    ~MemoryCopyFixture()
    {
        // Delete memory chunks
        for (auto buffer : buffers)
            free(buffer);
        buffers.clear();
    }
};

BENCHMARK_FIXTURE(MemoryCopyFixture, "memcpy() " + std::to_string(chunks) + " * ", settings)
{
    size_t size = context.x();
    uint8_t destination[chunk_size_to];
    for (auto source : buffers)
    {
        std::memcpy(destination, source, size);
        context.metrics().AddBytes(size);
    }
}

BENCHMARK("memmove", settings)
{
    size_t size = context.x();
    uint8_t buffer[chunk_size_to];
    std::memmove(buffer, buffer + size / 4, size / 2);
    std::memmove(buffer + size / 2, buffer + size / 4, size / 2);
    context.metrics().AddBytes(context.x());
}

BENCHMARK_MAIN()
