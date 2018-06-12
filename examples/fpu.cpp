//
// Created by Ivan Shynkarenka on 03.08.2015
//

#include "benchmark/cppbenchmark.h"

#include <cmath>

const uint64_t operations = 1000000000;

BENCHMARK("sin()", operations)
{
    static double sum = 0;
    sum += sin(123.456);
}

BENCHMARK("cos()", operations)
{
    static double sum = 0;
    sum += cos(123.456);
}

BENCHMARK("tan()", operations)
{
    static double sum = 0;
    sum += tan(123.456);
}

BENCHMARK_MAIN()
