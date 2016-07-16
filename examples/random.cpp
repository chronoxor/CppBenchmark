//
// Created by Ivan Shynkarenka on 03.08.2015.
//

#include "benchmark/cppbenchmark.h"

BENCHMARK("rand()", 100000000)
{
    rand();
}

BENCHMARK("rand()-till-zero", Settings().Infinite())
{
    if (rand() == 0)
        context.Cancel();
}

BENCHMARK_MAIN()
