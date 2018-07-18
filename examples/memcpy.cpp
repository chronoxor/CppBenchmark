//
// Created by Ivan Shynkarenka on 11.05.2017
//

#include "benchmark/cppbenchmark.h"

#include <cstdlib>
#include <cstring>
#include <vector>

const int chunk_size_from = 1024;
const int chunk_size_to = 65536;
const auto settings = CppBenchmark::Settings().ParamRange(chunk_size_from, chunk_size_to, [](int from, int to, int& result) { int r = result; result *= 2; return r; });

class MemoryCopyFixture
{
protected:
    uint8_t* buffer;

    MemoryCopyFixture()
    {
        // Create memory buffer
        buffer = (uint8_t*)std::malloc(chunk_size_to);
        // Prepare memory buffer
        for (int j = 0; j < chunk_size_to; ++j)
            buffer[j] = (uint8_t)j;
    }

    ~MemoryCopyFixture()
    {
        // Delete memory buffer
        std::free(buffer);
    }
};

BENCHMARK_FIXTURE(MemoryCopyFixture, "memcpy", settings)
{
    uint64_t crc = 0;
    size_t size = context.x();
    uint8_t local[chunk_size_to];
    std::memcpy(local, buffer, size);
    crc += local[0];
    context.metrics().AddBytes(size);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK("memmove", settings)
{
    uint64_t crc = 0;
    size_t size = context.x();
    uint8_t buffer[chunk_size_to];
    std::memmove(buffer, buffer + size / 4, size / 2);
    std::memmove(buffer + size / 2, buffer + size / 4, size / 2);
    crc = buffer[0];
    context.metrics().AddBytes(context.x());
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_MAIN()
