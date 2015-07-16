//
// Created by Ivan Shynkarenka on 16.07.2015.
//

#ifndef CPPBENCHMARK_MACROS_H
#define CPPBENCHMARK_MACROS_H

#include "benchmark_function.h"
#include "launcher_console.h"

namespace CppBenchmark {

namespace Internals {

class BenchmarkRegistrator
{
public:
    BenchmarkRegistrator(std::shared_ptr<BenchmarkFunction> benchmark)
    {
        LauncherConsole::GetInstance().AddBenchmark(benchmark);
    }
};

} // namespace Internals

} // namespace CppBenchmark

#define BENCHMARK_INTERNAL_UNIQUE_NAME_LINE2(name, line) name##line
#define BENCHMARK_INTERNAL_UNIQUE_NAME_LINE(name, line) BENCHMARK_INTERNAL_UNIQUE_NAME_LINE2(name, line)
#define BENCHMARK_INTERNAL_UNIQUE_NAME(name) BENCHMARK_INTERNAL_UNIQUE_NAME_LINE(name, __LINE__)

#define BENCHMARK_MAIN()                                    \
int main(int argc, char** argv)                             \
{                                                           \
    LauncherConsole::GetInstance().Initialize(argc, argv);  \
    LauncherConsole::GetInstance().Launch();                \
    return 0;                                               \
}

#define BENCHMARK(name)                                                                                                                                                                                         \
static void BENCHMARK_INTERNAL_UNIQUE_NAME(__benchmark__)(Context& context);                                                                                                                              \
namespace { CppBenchmark::Internals::BenchmarkRegistrator BENCHMARK_INTERNAL_UNIQUE_NAME(benchmark_registrator)(std::make_shared<BenchmarkFunction>(name, &BENCHMARK_INTERNAL_UNIQUE_NAME(__benchmark__))); }   \
static void BENCHMARK_INTERNAL_UNIQUE_NAME(__benchmark__)(Context& context)

#endif // CPPBENCHMARK_MACROS_H
