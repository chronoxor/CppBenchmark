//
// Created by Ivan Shynkarenka on 16.07.2015.
//

#ifndef CPPBENCHMARK_MACROS_H
#define CPPBENCHMARK_MACROS_H

#include "launcher_console.h"

namespace CppBenchmark {

namespace Internals {

class BenchmarkRegistrator
{
public:
    explicit BenchmarkRegistrator(std::shared_ptr<Benchmark> benchmark)
    { LauncherConsole::GetInstance().AddBenchmark(benchmark); }
};

} // namespace Internals

} // namespace CppBenchmark

#define BENCHMARK_INTERNAL_UNIQUE_NAME_LINE2(name, line) name##line
#define BENCHMARK_INTERNAL_UNIQUE_NAME_LINE(name, line) BENCHMARK_INTERNAL_UNIQUE_NAME_LINE2(name, line)
#define BENCHMARK_INTERNAL_UNIQUE_NAME(name) BENCHMARK_INTERNAL_UNIQUE_NAME_LINE(name, __LINE__)

#define BENCHMARK_MAIN()\
int main(int argc, char** argv)\
{\
    CppBenchmark::LauncherConsole::GetInstance().Initialize(argc, argv);\
    CppBenchmark::LauncherConsole::GetInstance().Launch();\
    CppBenchmark::LauncherConsole::GetInstance().Report();\
    return 0;\
}

#define BENCHMARK(...)\
namespace CppBenchmark {\
    class BENCHMARK_INTERNAL_UNIQUE_NAME(__benchmark__) : public Benchmark\
    {\
    public:\
        using Benchmark::Benchmark;\
    protected:\
        void Run(Context& context) override;\
    };\
    Internals::BenchmarkRegistrator BENCHMARK_INTERNAL_UNIQUE_NAME(benchmark_registrator)(std::make_shared<BENCHMARK_INTERNAL_UNIQUE_NAME(__benchmark__)>(__VA_ARGS__));\
}\
void CppBenchmark::BENCHMARK_INTERNAL_UNIQUE_NAME(__benchmark__)::Run(CppBenchmark::Context& context)

#define BENCHMARK_FIXTURE(fixture, ...)\
namespace CppBenchmark {\
    class BENCHMARK_INTERNAL_UNIQUE_NAME(__benchmark__) : public Benchmark, public fixture\
    {\
    public:\
        using Benchmark::Benchmark;\
    protected:\
        void Run(Context& context) override;\
    };\
    Internals::BenchmarkRegistrator BENCHMARK_INTERNAL_UNIQUE_NAME(benchmark_registrator)(std::make_shared<BENCHMARK_INTERNAL_UNIQUE_NAME(__benchmark__)>(__VA_ARGS__));\
}\
void CppBenchmark::BENCHMARK_INTERNAL_UNIQUE_NAME(__benchmark__)::Run(Context& context)

#define BENCHMARK_THREADS(...)\
namespace CppBenchmark {\
    class BENCHMARK_INTERNAL_UNIQUE_NAME(__benchmark__) : public BenchmarkThreads\
    {\
    public:\
        using BenchmarkThreads::BenchmarkThreads;\
    protected:\
        void RunThread(ContextThread& context) override;\
    };\
    Internals::BenchmarkRegistrator BENCHMARK_INTERNAL_UNIQUE_NAME(benchmark_registrator)(std::make_shared<BENCHMARK_INTERNAL_UNIQUE_NAME(__benchmark__)>(__VA_ARGS__));\
}\
void CppBenchmark::BENCHMARK_INTERNAL_UNIQUE_NAME(__benchmark__)::RunThread(CppBenchmark::ContextThread& context)

#define BENCHMARK_THREADS_FIXTURE(fixture, ...)\
namespace CppBenchmark {\
    class BENCHMARK_INTERNAL_UNIQUE_NAME(__benchmark__) : public BenchmarkThreads, public fixture\
    {\
    public:\
        using BenchmarkThreads::BenchmarkThreads;\
    protected:\
        void RunThread(ContextThread& context) override;\
    };\
    Internals::BenchmarkRegistrator BENCHMARK_INTERNAL_UNIQUE_NAME(benchmark_registrator)(std::make_shared<BENCHMARK_INTERNAL_UNIQUE_NAME(__benchmark__)>(__VA_ARGS__));\
}\
void CppBenchmark::BENCHMARK_INTERNAL_UNIQUE_NAME(__benchmark__)::RunThread(ContextThread& context)

#define BENCHMARK_CLASS(type, ...)\
namespace CppBenchmark { Internals::BenchmarkRegistrator BENCHMARK_INTERNAL_UNIQUE_NAME(benchmark_registrator)(std::make_shared<type>(__VA_ARGS__)); }

#endif // CPPBENCHMARK_MACROS_H
