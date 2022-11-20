//
// Created by Ivan Shynkarenka on 03.08.2015
//

#include "benchmark/cppbenchmark.h"

#include <cmath>

BENCHMARK("sin()")
{
    [[maybe_unused]] static double sum = 0;
    sum += sin(123.456);
}

BENCHMARK("cos()")
{
    [[maybe_unused]] static double sum = 0;
    sum += cos(123.456);
}

BENCHMARK("tan()")
{
    [[maybe_unused]] static double sum = 0;
    sum += tan(123.456);
}

BENCHMARK_MAIN()
