//
// Created by Ivan Shynkarenka on 16.07.2015.
//

#ifndef CPPBENCHMARK_BENCHMARK_FUNCTION_H
#define CPPBENCHMARK_BENCHMARK_FUNCTION_H

#include "benchmark.h"

namespace CppBenchmark {

class BenchmarkFunction : public Benchmark
{
public:
    BenchmarkFunction(const std::string& name, std::function<void (Context&)> function, const Settings& settings = Settings::Default)
            : Benchmark(name, settings),
              _function(function)
    {}
    BenchmarkFunction(const BenchmarkFunction&) = delete;
    BenchmarkFunction(BenchmarkFunction&&) = delete;
    virtual ~BenchmarkFunction() = default;

    BenchmarkFunction& operator=(const BenchmarkFunction&) = delete;
    BenchmarkFunction& operator=(BenchmarkFunction&&) = delete;

protected:
    void Run(Context& context) override { _function(context); }

private:
    std::function<void (Context&)> _function;
};

} // namespace CppBenchmark

#endif // CPPBENCHMARK_BENCHMARK_FUNCTION_H
