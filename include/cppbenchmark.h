/*!
    \file cppbenchmark.h
    \brief CppBenchmark definitions
    \author Ivan Shynkarenka
    \date 16.07.2015
    \copyright MIT License
*/

/*! \mainpage My Personal Index Page

C++ Benchmark Library allows to create performance benchmarks of some  code  to
investigate average/minimal/maximal execution time, items processing processing
speed, I/O throughput. CppBenchmark library has lots of features and allows  to
make benchmarks for different  kind  of  scenarios  such  as  micro-benchmarks,
benchmarks    with    fixtures    and    parameters,    threads     benchmarks,
produsers/consummers pattern.

This document contains CppBenchmark API references.

Library description, features, requirements and usage examples can be  find  on
GitHub: https://github.com/chronoxor/CppBenchmark

*/

#ifndef CPPBENCHMARK_H
#define CPPBENCHMARK_H

#include <sstream>

#include "executor.h"
#include "launcher_console.h"
#include "reporter_console.h"
#include "reporter_csv.h"
#include "reporter_json.h"

namespace CppBenchmark {

//! @cond
namespace Internals {

class BenchmarkRegistrator
{
public:
    explicit BenchmarkRegistrator(std::shared_ptr<Benchmark> benchmark)
    { LauncherConsole::GetInstance().AddBenchmark(benchmark); }
};

} // namespace Internals
//! @endcond

} // namespace CppBenchmark

//! @cond
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
    CppBenchmark::LauncherConsole::GetInstance().Launch();\
    CppBenchmark::LauncherConsole::GetInstance().Report();\
    return 0;\
}

//! Benchmark register macro
/*!
    Register a new benchmark with a given \a name and settings. Next to the definition you should provide a benchmark
    code.

    Example:
    \code{.cpp}
    // This benchmark will call MyTest() function in 1000000 iterations
    BENCHMARK("MyTestBenchmark", 1000000)
    {
        MyTest();
    }
    \endcode
*/
#define BENCHMARK(name, ...)\
namespace CppBenchmark {\
    class BENCHMARK_INTERNAL_UNIQUE_NAME(__benchmark__) : public Benchmark\
    {\
    public:\
        using Benchmark::Benchmark;\
    protected:\
        void Run(Context& context) override;\
    };\
    Internals::BenchmarkRegistrator BENCHMARK_INTERNAL_UNIQUE_NAME(benchmark_registrator)(std::make_shared<BENCHMARK_INTERNAL_UNIQUE_NAME(__benchmark__)>(name, Benchmark::TSettings(__VA_ARGS__)));\
}\
void CppBenchmark::BENCHMARK_INTERNAL_UNIQUE_NAME(__benchmark__)::Run(CppBenchmark::Context& context)

//! Benchmark with fixture register macro
/*!
    Register a new benchmark with a given \a fixture, \a name and settings. Next to the definition you should provide
    a benchmark code. Benchmark fixture is a user class that will be constructed before benchmarking and destructed
    after. In benchmark code you can access to public and protected fields & methods of the fixture.

    Example:
    \code{.cpp}
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
*/
#define BENCHMARK_FIXTURE(fixture, name, ...)\
namespace CppBenchmark {\
    class BENCHMARK_INTERNAL_UNIQUE_NAME(__benchmark__) : public Benchmark, public fixture\
    {\
    public:\
        using Benchmark::Benchmark;\
    protected:\
        void Run(Context& context) override;\
    };\
    Internals::BenchmarkRegistrator BENCHMARK_INTERNAL_UNIQUE_NAME(benchmark_registrator)(std::make_shared<BENCHMARK_INTERNAL_UNIQUE_NAME(__benchmark__)>(name, Benchmark::TSettings(__VA_ARGS__)));\
}\
void CppBenchmark::BENCHMARK_INTERNAL_UNIQUE_NAME(__benchmark__)::Run(Context& context)

//! Benchmark threads register macro
/*!
    Register a new threads benchmark with a given \a name and settings. Next to the definition you should provide
    a benchmark code that will be executed in multi-thread environment. You can use \a settings parameter to give
    threads count to want to measure with.

    Example:
    \code{.cpp}
    // This benchmark will output random value in std::cout 1000000 times in 4 concurrent threads environment
    BENCHMARK_THREADS("ThreadsConsoleBenchmark", 1000000, 4)
    {
        std::count << rand();
    }
    \endcode
*/
#define BENCHMARK_THREADS(name, ...)\
namespace CppBenchmark {\
    class BENCHMARK_INTERNAL_UNIQUE_NAME(__benchmark__) : public BenchmarkThreads\
    {\
    public:\
        using BenchmarkThreads::BenchmarkThreads;\
    protected:\
        void RunThread(ContextThread& context) override;\
    };\
    Internals::BenchmarkRegistrator BENCHMARK_INTERNAL_UNIQUE_NAME(benchmark_registrator)(std::make_shared<BENCHMARK_INTERNAL_UNIQUE_NAME(__benchmark__)>(name, BenchmarkThreads::TSettings(__VA_ARGS__)));\
}\
void CppBenchmark::BENCHMARK_INTERNAL_UNIQUE_NAME(__benchmark__)::RunThread(CppBenchmark::ContextThread& context)

//! Benchmark threads with fixture register macro
/*!
    Register a new threads benchmark with a given \a fixture, \a name and settings. Next to the definition you should
    provide a benchmark code that will be executed in multi-thread environment. Benchmark fixture is a user class
    that will be constructed before benchmarking and destructed after. In benchmark code you can access to public
    and protected fields & methods of the fixture. You can use \a settings parameter to give threads count to want
    to measure with.

    Example:
    \code{.cpp}
    class AtomicFixture
    {
    protected:
        std::atomic<int> counter;
    };

    // This benchmark will increment atomic counter 1000000 times in 4 concurrent threads environment
    BENCHMARK_THREADS_FIXTURE(AtomicFixture, "ThreadsAtomicIncrementBenchmark", 1000000, 4)
    {
        counter++;
    }
    \endcode
*/
#define BENCHMARK_THREADS_FIXTURE(fixture, name, ...)\
namespace CppBenchmark {\
    class BENCHMARK_INTERNAL_UNIQUE_NAME(__benchmark__) : public BenchmarkThreads, public fixture\
    {\
    public:\
        using BenchmarkThreads::BenchmarkThreads;\
    protected:\
        void RunThread(ContextThread& context) override;\
    };\
    Internals::BenchmarkRegistrator BENCHMARK_INTERNAL_UNIQUE_NAME(benchmark_registrator)(std::make_shared<BENCHMARK_INTERNAL_UNIQUE_NAME(__benchmark__)>(name, BenchmarkThreads::TSettings(__VA_ARGS__)));\
}\
void CppBenchmark::BENCHMARK_INTERNAL_UNIQUE_NAME(__benchmark__)::RunThread(ContextThread& context)

//! Benchmark class register macro
/*!
    Register a new benchmark based on a child class of a \a type with a given \a name and \a settings. You should
    inherit \a type from Benchmark, BenchmarkThreads or BenchmarkPC and implement all necessary benchmark methods.

    Example:
    \code{.cpp}
    class VectorBenchmark
    {
    protected:
        std::vector<int> container;

        void Initialize(Context& context) { container.reserve(1000); }
        void Run(Context& context) override { container.push_back(rand()); }
        void Cleanup(Context& context) override { container.clear(); }
    };

    // This benchmark will measure VectorBenchmark with 1000000 iterations
    BENCHMARK_CLASS(VectorBenchmark, "VectorPushBackBenchmark", 1000000)
    \endcode
*/
#define BENCHMARK_CLASS(type, name, ...)\
namespace CppBenchmark { Internals::BenchmarkRegistrator BENCHMARK_INTERNAL_UNIQUE_NAME(benchmark_registrator)(std::make_shared<type>(name, type::TSettings(__VA_ARGS__))); }

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
#define BENCHCODE_SCOPE(name) CppBenchmark::Executor::GetInstance().ScopeBenchmark(name);

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
#define BENCHCODE_START(name) CppBenchmark::Executor::GetInstance().StartBenchmark(name);

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
#define BENCHCODE_STOP(name) CppBenchmark::Executor::GetInstance().StopBenchmark(name);

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
    CppBenchmark::Executor::GetInstance().Report(BENCHMARK_INTERNAL_UNIQUE_NAME(reporter));\
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
    CppBenchmark::Executor::GetInstance().Report(BENCHMARK_INTERNAL_UNIQUE_NAME(reporter));\
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
    CppBenchmark::Executor::GetInstance().Report(BENCHMARK_INTERNAL_UNIQUE_NAME(reporter));\
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
    CppBenchmark::Executor::GetInstance().Report(BENCHMARK_INTERNAL_UNIQUE_NAME(reporter));\
    value = BENCHMARK_INTERNAL_UNIQUE_NAME(output).str();\
}

#endif // CPPBENCHMARK_H
