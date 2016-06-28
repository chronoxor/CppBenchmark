//
// Created by Ivan Shynkarenka on 18.07.2015.
//

#include "benchmark/cppbenchmark.h"

#include <array>

const int iterations = 100000;
const int chunk_size_from = 32;
const int chunk_size_to = 4096;
const auto settings = CppBenchmark::Settings().Iterations(iterations).ParamRange(chunk_size_from, chunk_size_to, [](int from, int to, int& result) { int r = result; result *= 2; return r; });

class FileFixture
{
public:
    FileFixture()
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

protected:
    FILE* file;
    std::array<char, chunk_size_to> buffer;
};

BENCHMARK_FIXTURE(FileFixture, "fwrite", settings)
{
    fwrite(buffer.data(), sizeof(char), context.x(), file);
    context.metrics().AddBytes(context.x());
}

BENCHMARK_FIXTURE(FileFixture, "fwrite+fflush", settings)
{
    fwrite(buffer.data(), sizeof(char), context.x(), file);
    fflush(file);
    context.metrics().AddBytes(context.x());
}

BENCHMARK_MAIN()
