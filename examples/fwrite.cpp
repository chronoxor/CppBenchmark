//
// Created by Ivan Shynkarenka on 18.07.2015
//

#include "benchmark/cppbenchmark.h"

#include <array>

const int chunk_size_from = 32;
const int chunk_size_to = 4096;
const auto settings = CppBenchmark::Settings().ParamRange(chunk_size_from, chunk_size_to, [](int from, int to, int& result) { int r = result; result *= 2; return r; });

class FileFixture
{
protected:
    FILE* file;
    std::array<uint8_t, chunk_size_to> buffer;

    FileFixture() : file(nullptr), buffer()
    {
        // Open file for binary write
        file = fopen("fwrite.out", "wb");
    }

    ~FileFixture()
    {
        // Close file
        fclose(file);

        // Delete file
        remove("fwrite.out");
    }
};

BENCHMARK_FIXTURE(FileFixture, "fwrite()", settings)
{
    fwrite(buffer.data(), sizeof(uint8_t), context.x(), file);
    context.metrics().AddBytes(context.x());
}

BENCHMARK_FIXTURE(FileFixture, "fwrite()+fflush()", settings)
{
    fwrite(buffer.data(), sizeof(uint8_t), context.x(), file);
    fflush(file);
    context.metrics().AddBytes(context.x());
}

BENCHMARK_MAIN()
