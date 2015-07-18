//
// Created by Ivan Shynkarenka on 18.07.2015.
//

#ifndef CPPBENCHMARK_BENCHMARK_FIXTURE_H
#define CPPBENCHMARK_BENCHMARK_FIXTURE_H

#include "context.h"

namespace CppBenchmark {

class BenchmarkFixture
{
public:
    BenchmarkFixture() = default;
    BenchmarkFixture(const BenchmarkFixture&) = default;
    BenchmarkFixture(BenchmarkFixture&&) = default;
    virtual ~BenchmarkFixture() = default;

    BenchmarkFixture& operator=(const BenchmarkFixture&) = default;
    BenchmarkFixture& operator=(BenchmarkFixture&&) = default;

protected:
    virtual void Initialize(Context& context) {}
    virtual void Cleanup(Context& context) {}
};

} // namespace CppBenchmark
#endif // CPPBENCHMARK_BENCHMARK_FIXTURE_H
