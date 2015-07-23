//
// Created by Ivan Shynkarenka on 18.07.2015.
//

#include "macros.h"

#include <array>

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
    std::array<char, 1048576> buffer;
};

BENCHMARK_FIXTURE(FileFixture, "fwrite", Settings().ParamRange(32, 4096, [](int from, int to, int result) {return (result * 2); }).Iterations(100000))
{
    fwrite(buffer.data(), sizeof(char), context.x(), file);
    context.metrics().AddBytes(context.x());
}

BENCHMARK_FIXTURE(FileFixture, "fwrite+fflush", Settings().ParamRange(32, 4096, [](int from, int to, int result) {return (result * 2); }).Iterations(100000))
{
    fwrite(buffer.data(), sizeof(char), context.x(), file);
    fflush(file);
    context.metrics().AddBytes(context.x());
}

BENCHMARK_MAIN()
