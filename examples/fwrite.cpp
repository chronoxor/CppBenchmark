//
// Created by Ivan Shynkarenka on 18.07.2015.
//

#include "macros.h"

#include <algorithm>
#include <array>

class FileFixture
{
public:
    FileFixture()
    {
        // Fill buffer with a random bytes
        std::generate(_buffer.begin(), _buffer.end(), std::rand);

        // Open file for binary write
        _file = fopen("fwrite.out", "wb");
    }

    ~FileFixture()
    {
        // Close file
        fclose(_file);

        // Delete file
        remove("fwrite.out");
    }

protected:
    FILE* _file;
    std::array<char, 1048576> _buffer;
};

BENCHMARK_FIXTURE(FileFixture, "fwrite", Settings().ParamRange(32, 4096, [](int from, int to, int result) {return (result > 0) ? (result * 2) : from; }).Iterations(100000))
{
    fwrite(_buffer.data(), sizeof(char), context.x(), _file);
    context.metrics().AddBytes(context.x());
}

BENCHMARK_FIXTURE(FileFixture, "fwrite+fflush", Settings().ParamRange(32, 4096, [](int from, int to, int result) {return (result > 0) ? (result * 2) : from; }).Iterations(100000))
{
    fwrite(_buffer.data(), sizeof(char), context.x(), _file);
    fflush(_file);
    context.metrics().AddBytes(context.x());
}

BENCHMARK_MAIN()
