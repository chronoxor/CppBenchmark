/*!
    \file cppbenchmark.h
    \brief CppBenchmark definitions
    \author Ivan Shynkarenka
    \date 16.07.2015
    \copyright MIT License
*/

#ifndef CPPBENCHMARK_H
#define CPPBENCHMARK_H

#include "benchmark/executor.h"
#include "benchmark/launcher_console.h"
#include "benchmark/reporter_console.h"
#include "benchmark/reporter_csv.h"
#include "benchmark/reporter_json.h"

namespace CppBenchmark {

//! @cond INTERNALS
namespace Internals {

class BenchmarkRegistrator
{
public:
    explicit BenchmarkRegistrator(const std::function<std::shared_ptr<BenchmarkBase>()>& builder)
    { LauncherConsole::GetInstance().AddBenchmarkBuilder(builder); }
};

} // namespace Internals
//! @endcond

} // namespace CppBenchmark

//! @cond INTERNALS
#define BENCHMARK_INTERNAL_UNIQUE_NAME_LINE2(name, line) name##line
#define BENCHMARK_INTERNAL_UNIQUE_NAME_LINE(name, line) BENCHMARK_INTERNAL_UNIQUE_NAME_LINE2(name, line)
#define BENCHMARK_INTERNAL_UNIQUE_NAME(name) BENCHMARK_INTERNAL_UNIQUE_NAME_LINE(name, __LINE__)
//! @endcond

//! Benchmark main entry point macro
/*!
    Main entry point definition for all benchmarks. Place this macro in some .cpp file to provide a main() function
    with registered LauncherConsole with \a argc & \a argv arguments parsing.
*/
#define BENCHMARK_MAIN()\
int main(int argc, char** argv)\
{\
    CppBenchmark::LauncherConsole::GetInstance().Initialize(argc, argv);\
    CppBenchmark::LauncherConsole::GetInstance().Execute();\
    CppBenchmark::LauncherConsole::GetInstance().Report();\
    return 0;\
}

//! Benchmark register macro
/*!
    Register a new benchmark with a given name and settings. Next to the definition you should provide a benchmark code.

    Example:
    \code{.cpp}
    // This benchmark will call MyTest() function in 1000000 operations
    BENCHMARK("MyTestBenchmark", 1000000)
    {
        MyTest();
    }
    \endcode
*/
#define BENCHMARK(...)\
namespace CppBenchmark {\
    class BENCHMARK_INTERNAL_UNIQUE_NAME(__benchmark__) : public Benchmark\
    {\
    public:\
        using Benchmark::Benchmark;\
    protected:\
        void Run(Context& context) override;\
    };\
    Internals::BenchmarkRegistrator BENCHMARK_INTERNAL_UNIQUE_NAME(benchmark_registrator)([]() { return std::make_shared<BENCHMARK_INTERNAL_UNIQUE_NAME(__benchmark__)>(__VA_ARGS__); });\
}\
void CppBenchmark::BENCHMARK_INTERNAL_UNIQUE_NAME(__benchmark__)::Run(CppBenchmark::Context& context)

//! Benchmark with fixture register macro
/*!
    Register a new benchmark with a given \a fixture, name and settings. Next to the definition you should provide a benchmark
    code. In benchmark code you can access to public and protected fields and methods of the fixture.

    Benchmark static fixture is a user class that will be constructed before benchmarking and will be destructed after.

    Static fixture example:
    \code{.cpp}
    // Vector fixture will be created only once and its field will be visible in benchmark body!
    class VectorFixture
    {
    protected:
        std::vector<int> container;
    };

    // This benchmark will insert random value into std::vector<int> 1000000 times
    BENCHMARK_FIXTURE(VectorFixture, "VectorPushBackBenchmark", 1000000)
    {
        container.push_back(rand());
    }
    \endcode

    Benchmark dynamic fixture is a user class inherited from Fixture base class and overrides Initialize()/Cleanup() methods
    that will be called before/after each benchmark attempt.

    Dynamic fixture example:
    \code{.cpp}
    // Vector fixture will be created only once and its filed will be visible in benchmark body!
    class VectorFixture : public virtual CppBenchmark::Fixture
    {
    protected:
        std::vector<int> container;

        // Initialize method will called just before each benchmark attempt
        void Initialize(CppBenchmark::Context& context) override { container = std::vector<int>(); }
        // Cleanup method will called just after each benchmark attempt
        void Cleanup(CppBenchmark::Context& context) override { container.clear(); }
    };

    // This benchmark will insert random value into std::vector<int> 1000000 times
    BENCHMARK_FIXTURE(VectorFixture, "VectorPushBackBenchmark", 1000000)
    {
        container.push_back(rand());
    }
    \endcode
*/
#define BENCHMARK_FIXTURE(fixture, ...)\
namespace CppBenchmark {\
    class BENCHMARK_INTERNAL_UNIQUE_NAME(__benchmark__) : public Benchmark, public fixture\
    {\
    public:\
        using Benchmark::Benchmark;\
    protected:\
        void Run(Context& context) override;\
    };\
    Internals::BenchmarkRegistrator BENCHMARK_INTERNAL_UNIQUE_NAME(benchmark_registrator)([]() { return std::make_shared<BENCHMARK_INTERNAL_UNIQUE_NAME(__benchmark__)>(__VA_ARGS__); });\
}\
void CppBenchmark::BENCHMARK_INTERNAL_UNIQUE_NAME(__benchmark__)::Run(Context& context)

//! Benchmark threads register macro
/*!
    Register a new threads benchmark with a given name and settings. Next to the definition you should provide
    a benchmark code that will be executed in multi-thread environment. You can use \a settings parameter to give
    threads count you want to measure with.

    Example:
    \code{.cpp}
    // This benchmark will output random value in std::cout 1000000 times in 4 concurrent threads environment
    BENCHMARK_THREADS("ThreadsConsoleBenchmark", 4, 1000000)
    {
        std::count << rand();
    }
    \endcode
*/
#define BENCHMARK_THREADS(...)\
namespace CppBenchmark {\
    class BENCHMARK_INTERNAL_UNIQUE_NAME(__benchmark__) : public BenchmarkThreads\
    {\
    public:\
        using BenchmarkThreads::BenchmarkThreads;\
    protected:\
        void RunThread(ContextThreads& context) override;\
    };\
    Internals::BenchmarkRegistrator BENCHMARK_INTERNAL_UNIQUE_NAME(benchmark_registrator)([]() { return std::make_shared<BENCHMARK_INTERNAL_UNIQUE_NAME(__benchmark__)>(__VA_ARGS__); });\
}\
void CppBenchmark::BENCHMARK_INTERNAL_UNIQUE_NAME(__benchmark__)::RunThread(CppBenchmark::ContextThreads& context)

//! Benchmark threads with fixture register macro
/*!
    Register a new threads benchmark with a given \a fixture, name and settings. Next to the definition you should
    provide a benchmark code that will be executed in multi-thread environment. In benchmark code you can access to
    public and protected fields & methods of the fixture. You can use \a settings parameter to give threads count
    you want to measure with.

    Benchmark static threads fixture is a user class that will be constructed before benchmarking and destructed after.

    Static threads fixture example:
    \code{.cpp}
    // Atomic threads fixture will be created only once and its field will be visible in benchmark body!
    class AtomicPreset
    {
    protected:
        std::atomic<int> counter;
    };

    // This benchmark will increment atomic counter 1000000 times in 4 concurrent threads environment
    BENCHMARK_THREADS_PRESET(AtomicPreset, "ThreadsAtomicIncrementBenchmark", 4, 1000000)
    {
        counter++;
    }
    \endcode

    Benchmark dynamic threads fixture is a user class inherited from Fixture or FixtureThreads base class and
    overrides Initialize()/Cleanup()/InitializeThread()/CleanupThread() methods that will be called before/after
    each benchmark attempt and before/after each thread is started/finished.

    Dynamic threads fixture example:
    \code{.cpp}
    // Atomic threads fixture will be created only once and its field will be visible in benchmark body!
    class AtomicFixture : public virtual CppBenchmark::FixtureThreads
    {
    protected:
        std::atomic<int> counter;

        // Initialize method will called just before each benchmark attempt
        void Initialize(CppBenchmark::ContextThreads& context) override { counter = 123; }
        // Cleanup method will called just after each benchmark attempt
        void Cleanup(CppBenchmark::ContextThreads& context) override { counter = 987; }

        // Initialize thread method will called just before each benchmark thread is started
        void InitializeThread(CppBenchmark::ContextThreads& context) override { counter = context.threads(); }
        // Cleanup thread method will called just after each benchmark thread is finished
        void CleanupThread(CppBenchmark::ContextThreads& context) override { counter = 0; }
    };

    // This benchmark will increment atomic counter 1000000 times in 4 concurrent threads environment
    BENCHMARK_THREADS_FIXTURE(AtomicFixture, "ThreadsAtomicIncrementBenchmark", 4, 1000000)
    {
        counter++;
    }
    \endcode
*/
#define BENCHMARK_THREADS_FIXTURE(fixture, ...)\
namespace CppBenchmark {\
    class BENCHMARK_INTERNAL_UNIQUE_NAME(__benchmark__) : public BenchmarkThreads, public fixture\
    {\
    public:\
        using BenchmarkThreads::BenchmarkThreads;\
    protected:\
        void RunThread(ContextThreads& context) override;\
    };\
    Internals::BenchmarkRegistrator BENCHMARK_INTERNAL_UNIQUE_NAME(benchmark_registrator)([]() { return std::make_shared<BENCHMARK_INTERNAL_UNIQUE_NAME(__benchmark__)>(__VA_ARGS__); });\
}\
void CppBenchmark::BENCHMARK_INTERNAL_UNIQUE_NAME(__benchmark__)::RunThread(ContextThreads& context)

//! Benchmark class register macro
/*!
    Register a new benchmark based on a child class of a \a type with a given name and \a settings. You should
    inherit \a type from Benchmark, BenchmarkThreads or BenchmarkPC and implement all necessary benchmark methods.

    Example:
    \code{.cpp}
    class VectorBenchmark
    {
    protected:
        std::vector<int> container;

        void Initialize(CppBenchmark::Context& context) { container.reserve(1000); }
        void Run(CppBenchmark::Context& context) override { container.push_back(rand()); }
        void Cleanup(CppBenchmark::Context& context) override { container.clear(); }
    };

    // This benchmark will measure VectorBenchmark with 1000000 operations
    BENCHMARK_CLASS(VectorBenchmark, "VectorPushBackBenchmark", 1000000)
    \endcode
*/
#define BENCHMARK_CLASS(type, ...)\
namespace CppBenchmark { Internals::BenchmarkRegistrator BENCHMARK_INTERNAL_UNIQUE_NAME(benchmark_registrator)([]() { return std::make_shared<type>(__VA_ARGS__); }); }

//! Dynamic benchmark scope register macro
/*!
    Create a scope guard for dynamic benchmark with the given \a name. It will be automatically registered in static
    Executor class.

    Example:
    \code{.cpp}
    // Some scope...
    {
        auto benchmark = BENCHCODE_SCOPE("My dynamic benchmark");

        // Here goes some code fragment you want to benchmark dynamically...
        ...
    }
    \endcode
*/
#define BENCHCODE_SCOPE(name) CppBenchmark::Executor::ScopeBenchmark(name);

//! Dynamic benchmark start macro
/*!
    Start dynamic benchmark with the given \a name. It will be automatically registered in static Executor class.
    Should be used in pair with BENCHCODE_STOP macro!

    Example:
    \code{.cpp}
    BENCHCODE_START("My dynamic benchmark");

    // Here goes some code fragment you want to benchmark dynamically...
    ...

    BENCHCODE_STOP("My dynamic benchmark");
    \endcode
*/
#define BENCHCODE_START(name) CppBenchmark::Executor::StartBenchmark(name);

//! Dynamic benchmark stop macro
/*!
    Stop dynamic benchmark with the given \a name. It will be automatically registered in static Executor class.
    Should be used in pair with BENCHCODE_START macro!

    Example:
    \code{.cpp}
    BENCHCODE_START("My dynamic benchmark");

    // Here goes some code fragment you want to benchmark dynamically...
    ...

    BENCHCODE_STOP("My dynamic benchmark");
    \endcode
*/
#define BENCHCODE_STOP(name) CppBenchmark::Executor::StopBenchmark(name);

//! Dynamic benchmarks report to console macro
/*!
    Prepare results of all dynamic benchmarks registered in static Executor class and show them using ReporterConsole.

    Example:
    \code{.cpp}
    int main(int argc, char** argv)
    {
        // Do some calculations or program run and measure code fragments with dynamic benchmarks...
        ...

        // Report benchmark results
        BENCHCODE_REPORT();

        return 0;
    }
    \endcode
*/
#define BENCHCODE_REPORT()\
{\
    CppBenchmark::ReporterConsole BENCHMARK_INTERNAL_UNIQUE_NAME(reporter);\
    CppBenchmark::Executor::Report(BENCHMARK_INTERNAL_UNIQUE_NAME(reporter));\
}

//! Dynamic benchmarks report as a string macro
/*!
    Prepare results of all dynamic benchmarks registered in static Executor class using ReporterConsole and store them
    in a string variable \a value.

    Example:
    \code{.cpp}
    // Report benchmark results
    std::string report;
    BENCHCODE_REPORT_STR(report);
    \endcode
*/
#define BENCHCODE_REPORT_STR(value)\
{\
    std::ostringstream BENCHMARK_INTERNAL_UNIQUE_NAME(output);\
    CppBenchmark::ReporterConsole BENCHMARK_INTERNAL_UNIQUE_NAME(reporter)(BENCHMARK_INTERNAL_UNIQUE_NAME(output));\
    CppBenchmark::Executor::Report(BENCHMARK_INTERNAL_UNIQUE_NAME(reporter));\
    value = BENCHMARK_INTERNAL_UNIQUE_NAME(output).str();\
}

//! Dynamic benchmarks report as a CSV format string macro
/*!
    Prepare results of all dynamic benchmarks registered in static Executor class using ReporterCSV and store them
    in a string variable \a value.

    Example:
    \code{.cpp}
    // Report benchmark results in CSV format
    std::string report;
    BENCHCODE_REPORT_CSV(report);
    \endcode
*/
#define BENCHCODE_REPORT_CSV(value)\
{\
    std::ostringstream BENCHMARK_INTERNAL_UNIQUE_NAME(output);\
    CppBenchmark::ReporterCSV BENCHMARK_INTERNAL_UNIQUE_NAME(reporter)(BENCHMARK_INTERNAL_UNIQUE_NAME(output));\
    CppBenchmark::Executor::Report(BENCHMARK_INTERNAL_UNIQUE_NAME(reporter));\
    value = BENCHMARK_INTERNAL_UNIQUE_NAME(output).str();\
}

//! Dynamic benchmarks report as a JSON format string macro
/*!
    Prepare results of all dynamic benchmarks registered in static Executor class using ReporterJSON and store them
    in a string variable \a value.

    Example:
    \code{.cpp}
    // Report benchmark results in JSON format
    std::string report;
    BENCHCODE_REPORT_JSON(report);
    \endcode
*/
#define BENCHCODE_REPORT_JSON(value)\
{\
    std::ostringstream BENCHMARK_INTERNAL_UNIQUE_NAME(output);\
    CppBenchmark::ReporterJSON BENCHMARK_INTERNAL_UNIQUE_NAME(reporter)(BENCHMARK_INTERNAL_UNIQUE_NAME(output));\
    CppBenchmark::Executor::Report(BENCHMARK_INTERNAL_UNIQUE_NAME(reporter));\
    value = BENCHMARK_INTERNAL_UNIQUE_NAME(output).str();\
}

#endif // CPPBENCHMARK_H
