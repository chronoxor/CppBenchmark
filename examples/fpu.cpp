//
// Created by Ivan Shynkarenka on 03.08.2015.
//

#include "benchmark/cppbenchmark.h"

#include <math.h>

const int iterations = 1000000000;

BENCHMARK("sin", iterations)
{
    sin(123.456);
}

BENCHMARK("cos", iterations)
{
    cos(123.456);
}

BENCHMARK("tan", iterations)
{
    tan(123.456);
}

BENCHMARK_MAIN()
