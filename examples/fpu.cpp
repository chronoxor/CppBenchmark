//
// Created by Ivan Shynkarenka on 03.08.2015.
//

#include "benchmark/cppbenchmark.h"

#include <math.h>

const uint64_t iterations = 1000000000;

BENCHMARK("sin()", iterations)
{
    static double sum = 0;
    sum += sin(123.456);
}

BENCHMARK("cos()", iterations)
{
    static double sum = 0;
    sum += cos(123.456);
}

BENCHMARK("tan()", iterations)
{
    static double sum = 0;
    sum += tan(123.456);
}

BENCHMARK_MAIN()
