//
// Created by Ivan Shynkarenka on 03.08.2015
//

#include "benchmark/cppbenchmark.h"

BENCHMARK("rand()")
{
    rand();
}

BENCHMARK("rand()-till-zero", Settings().Infinite().Attempts(10))
{
    if (rand() == 0)
        context.Cancel();
}

BENCHMARK_MAIN()
