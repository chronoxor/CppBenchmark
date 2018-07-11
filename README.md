# CppBenchmark

[![Linux build status](https://img.shields.io/travis/chronoxor/CppBenchmark/master.svg?label=Linux)](https://travis-ci.org/chronoxor/CppBenchmark)
[![OSX build status](https://img.shields.io/travis/chronoxor/CppBenchmark/master.svg?label=OSX)](https://travis-ci.org/chronoxor/CppBenchmark)
[![Cygwin build status](https://img.shields.io/appveyor/ci/chronoxor/CppBenchmark/master.svg?label=Cygwin)](https://ci.appveyor.com/project/chronoxor/CppBenchmark)
[![MinGW build status](https://img.shields.io/appveyor/ci/chronoxor/CppBenchmark/master.svg?label=MinGW)](https://ci.appveyor.com/project/chronoxor/CppBenchmark)
[![Windows build status](https://img.shields.io/appveyor/ci/chronoxor/CppBenchmark/master.svg?label=Windows)](https://ci.appveyor.com/project/chronoxor/CppBenchmark)

C++ Benchmark Library allows to create performance benchmarks of some code to investigate
average/minimal/maximal execution time, items processing processing speed, I/O throughput.
CppBenchmark library has lots of [features](#features) and allows to make benchmarks for
[different kind of scenarios](#benchmark-examples) such as micro-benchmarks, benchmarks
with fixtures and parameters, threads benchmarks, produsers/consummers pattern.

[CppBenchmark API reference](https://chronoxor.github.io/CppBenchmark/index.html)

# Contents
  * [Features](#features)
  * [Requirements](#requirements)
  * [How to build?](#how-to-build)
    * [Clone repository with submodules](#clone-repository-with-submodules)
    * [Linux](#linux)
    * [OSX](#osx)
    * [Windows (Cygwin)](#windows-cygwin)
    * [Windows (MinGW)](#windows-mingw)
    * [Windows (MinGW with MSYS)](#windows-mingw-with-msys)
    * [Windows (Visual Studio)](#windows-visual-studio)
  * [How to create a benchmark?](#how-to-create-a-benchmark)
  * [Benchmark examples](#benchmark-examples)
    * [Example 1: Benchmark of a function call](#example-1-benchmark-of-a-function-call)
    * [Example 2: Benchmark with cancelation](#example-2-benchmark-with-cancelation)
    * [Example 3: Benchmark with static fixture](#example-3-benchmark-with-static-fixture)
    * [Example 4: Benchmark with dynamic fixture](#example-4-benchmark-with-dynamic-fixture)
    * [Example 5: Benchmark with parameters](#example-5-benchmark-with-parameters)
    * [Example 6: Benchmark class](#example-6-benchmark-class)
    * [Example 7: Benchmark I/O operations](#example-7-benchmark-io-operations)
    * [Example 8: Benchmark latency with auto update](#example-8-benchmark-latency-with-auto-update)
    * [Example 9: Benchmark latency with manual update](#example-9-benchmark-latency-with-manual-update)
    * [Example 10: Benchmark threads](#example-10-benchmark-threads)
    * [Example 11: Benchmark threads with fixture](#example-11-benchmark-threads-with-fixture)
    * [Example 12: Benchmark single producer, single consumer pattern](#example-12-benchmark-single-producer-single-consumer-pattern)
    * [Example 13: Benchmark multiple producers, multiple consumers pattern](#example-13-benchmark-multiple-producers-multiple-consumers-pattern)
    * [Example 14: Dynamic benchmarks](#example-14-dynamic-benchmarks)
  * [Command line options](#command-line-options)

# Features
* Cross platform (Linux, OSX, Windows)
* [Micro-benchmarks](#example-1-benchmark-of-a-function-call)
* Benchmarks with [static fixtures](#example-3-benchmark-with-static-fixture) and [dynamic fixtures](#example-4-benchmark-with-dynamic-fixture)
* Benchmarks with [parameters](#example-5-benchmark-with-parameters) (single, pair, triple parameters, ranges, ranges with selectors)
* [Benchmark infinite run with cancelation](#example-2-benchmark-with-cancelation)
* [Benchmark items processing speed](#example-6-benchmark-class)
* [Benchmark I/O throughput](#example-7-benchmark-io-operations)
* [Benchmark latency](#example-8-benchmark-latency-with-auto-update) with [High Dynamic Range (HDR) Histograms](https://hdrhistogram.github.io/HdrHistogram/)
* [Benchmark threads](#example-10-benchmark-threads)
* [Benchmark producers/consumers pattern](#example-12-benchmark-single-producer-single-consumer-pattern)
* Different reporting formats: console, csv, json
* Colored console progress and report

![Console colored report](https://github.com/chronoxor/CppBenchmark/raw/master/images/console.png)

# Requirements
* Linux
* OSX
* Windows 7 / Windows 10
* [CMake](https://www.cmake.org)
* [GIT](https://git-scm.com)
* [GCC](https://gcc.gnu.org)

Optional:
* [Clang](https://clang.llvm.org)
* [Clion](https://www.jetbrains.com/clion)
* [Cygwin](https://cygwin.com)
* [MinGW](https://mingw-w64.org/doku.php)
* [Visual Studio](https://www.visualstudio.com)

# How to build?

## Clone repository with submodules
```shell
git clone https://github.com/chronoxor/CppBenchmark.git CppBenchmark
cd CppBenchmark
git submodule update --init --recursive --remote
```

## Linux
```shell
cd build
./unix.sh
```

## OSX
```shell
cd build
./unix.sh
```

## Windows (Cygwin)
```shell
cd build
cygwin.bat
```

## Windows (MinGW)
```shell
cd build
mingw.bat
```

## Windows (Visual Studio)
```shell
cd build
vs.bat
```

# How to create a benchmark?
1. [Build CppBenchmark library](#how-to-build)
2. Create a new *.cpp file
3. Insert #include "cppbenchmark.h"
4. Add benchmark code (examples for different scenarios you can find below)
5. Insert BENCHMARK_MAIN() at the end
6. Compile the *.cpp file and link it with CppBenchmark library
7. Run it (see also possible [command line options](#command-line-options))

# Benchmark examples

## Example 1: Benchmark of a function call
```c++
#include "cppbenchmark.h"

#include <math.h>

// Benchmark sin() call for 1000000000 times.
// Make 5 attemtps (by default) and choose one with the best time result.
BENCHMARK("sin", 1000000000)
{
    sin(123.456);
}

BENCHMARK_MAIN()
```

Report fragment is the following:
```
===============================================================================
Benchmark: sin
Attempts: 5
Operations: 1000000000
-------------------------------------------------------------------------------
Phase: sin()
Average time: 2 ns/op
Minimal time: 2 ns/op
Maximal time: 2 ns/op
Total time: 2.428 s
Total operations: 1000000000
Operations throughput: 411732889 ops/s
===============================================================================
```

## Example 2: Benchmark with cancelation
```c++
#include "cppbenchmark.h"

// Benchmark rand() call until it returns 0.
// Benchmark will print operations count required to get 'rand() == 0' case.
// Make 10 attemtps and choose one with the best time result.
BENCHMARK("rand-till-zero", Settings().Infinite().Attempts(10))
{
    if (rand() == 0)
        context.Cancel();
}

BENCHMARK_MAIN()
```

Report fragment is the following:
```
===============================================================================
Benchmark: rand-till-zero
Attempts: 10
-------------------------------------------------------------------------------
Phase: rand-till-zero()
Average time: 25 ns/op
Minimal time: 25 ns/op
Maximal time: 513 ns/op
Total time: 94.234 mcs
Total operations: 3716
Operations throughput: 39433750 ops/s
===============================================================================
```

## Example 3: Benchmark with static fixture
Static fixture will be constructed once per each benchmark, will be the same for
each attempt / operation and will be destructed at the end of the benchmark.

```c++
#include "macros.h"

#include <list>
#include <vector>

const int operations = 1000;

template <typename T>
class ContainerFixture
{
protected:
    T container;

    ContainerFixture()
    {
        for (int i = 0; i < 1000000; ++i)
            container.push_back(rand());
    }
};

BENCHMARK_FIXTURE(ContainerFixture<std::list<int>>, "std::list<int>.forward", operations)
{
    for (auto it = container.begin(); it != container.end(); ++it)
        ++(*it);
}

BENCHMARK_FIXTURE(ContainerFixture<std::list<int>>, "std::list<int>.backward", operations)
{
    for (auto it = container.rbegin(); it != container.rend(); ++it)
        ++(*it);
}

BENCHMARK_FIXTURE(ContainerFixture<std::vector<int>>, "std::vector<int>.forward", operations)
{
    for (auto it = container.begin(); it != container.end(); ++it)
        ++(*it);
}

BENCHMARK_FIXTURE(ContainerFixture<std::vector<int>>, "std::vector<int>.backward", operations)
{
    for (auto it = container.rbegin(); it != container.rend(); ++it)
        ++(*it);
}

BENCHMARK_MAIN()
```

Report fragment is the following:
```
===============================================================================
Benchmark: std::list<int>.forward
Attempts: 5
Operations: 1000
-------------------------------------------------------------------------------
Phase: std::list<int>.forward()
Average time: 6.055 ms/op
Minimal time: 6.055 ms/op
Maximal time: 6.337 ms/op
Total time: 6.055 s
Total operations: 1000
Operations throughput: 165 ops/s
===============================================================================
Benchmark: std::list<int>.backward
Attempts: 5
Operations: 1000
-------------------------------------------------------------------------------
Phase: std::list<int>.backward()
Average time: 6.075 ms/op
Minimal time: 6.075 ms/op
Maximal time: 6.935 ms/op
Total time: 6.075 s
Total operations: 1000
Operations throughput: 164 ops/s
===============================================================================
Benchmark: std::vector<int>.forward
Attempts: 5
Operations: 1000
-------------------------------------------------------------------------------
Phase: std::vector<int>.forward()
Average time: 663.003 mcs/op
Minimal time: 663.003 mcs/op
Maximal time: 678.439 mcs/op
Total time: 663.003 ms
Total operations: 1000
Operations throughput: 1508 ops/s
===============================================================================
Benchmark: std::vector<int>.backward
Attempts: 5
Operations: 1000
-------------------------------------------------------------------------------
Phase: std::vector<int>.backward()
Average time: 667.515 mcs/op
Minimal time: 667.515 mcs/op
Maximal time: 681.929 mcs/op
Total time: 667.515 ms
Total operations: 1000
Operations throughput: 1498 ops/s
===============================================================================
```

## Example 4: Benchmark with dynamic fixture
Dynamic fixture can be used to prepare benchmark before each attempt with
Initialize() / Cleanup() methods. You can access to the current benchmark
context in dynamic fixture methods.

```c++
#include "macros.h"

#include <deque>
#include <list>
#include <vector>

const int operations = 10000000;

template <typename T>
class ContainerFixture : public virtual CppBenchmark::Fixture
{
protected:
    T container;

    void Initialize(CppBenchmark::Context& context) override { container = T(); }
    void Cleanup(CppBenchmark::Context& context) override { container.clear(); }
};

BENCHMARK_FIXTURE(ContainerFixture<std::list<int>>, "std::list<int>.push_back", operations)
{
    container.push_back(0);
}

BENCHMARK_FIXTURE(ContainerFixture<std::vector<int>>, "std::vector<int>.push_back", operations)
{
    container.push_back(0);
}

BENCHMARK_FIXTURE(ContainerFixture<std::deque<int>>, "std::deque<int>.push_back", operations)
{
    container.push_back(0);
}

BENCHMARK_MAIN()
```

Report fragment is the following:
```
===============================================================================
Benchmark: std::list<int>.push_back
Attempts: 5
Operations: 10000000
-------------------------------------------------------------------------------
Phase: std::list<int>.push_back()
Average time: 50 ns/op
Minimal time: 50 ns/op
Maximal time: 53 ns/op
Total time: 505.169 ms
Total operations: 10000000
Operations throughput: 19795348 ops/s
===============================================================================
Benchmark: std::vector<int>.push_back
Attempts: 5
Operations: 10000000
-------------------------------------------------------------------------------
Phase: std::vector<int>.push_back()
Average time: 9 ns/op
Minimal time: 9 ns/op
Maximal time: 10 ns/op
Total time: 97.252 ms
Total operations: 10000000
Operations throughput: 102824688 ops/s
===============================================================================
Benchmark: std::deque<int>.push_back
Attempts: 5
Operations: 10000000
-------------------------------------------------------------------------------
Phase: std::deque<int>.push_back()
Average time: 21 ns/op
Minimal time: 21 ns/op
Maximal time: 22 ns/op
Total time: 218.826 ms
Total operations: 10000000
Operations throughput: 45698221 ops/s
===============================================================================
```

## Example 5: Benchmark with parameters
Additional parameters can be provided to benchmark with settings using fluent
syntax. Parameters can be single, pair or tripple, provided as a value, as a
range, or with a range and selector function. Benchmark will be launched for
each parameters combination.

```c++
#include "cppbenchmark.h"

#include <algorithm>
#include <vector>

class SortFixture : public virtual CppBenchmark::Fixture
{
protected:
    std::vector<int> items;

    void Initialize(CppBenchmark::Context& context) override
    {
        items.resize(context.x());
        std::generate(items.begin(), items.end(), rand);
    }

    void Cleanup(CppBenchmark::Context& context) override
    {
        items.clear();
    }
};

BENCHMARK_FIXTURE(SortFixture, "std::sort", Settings().Param(1000000).Param(10000000))
{
    std::sort(items.begin(), items.end());
    context.metrics().AddItems(items.size());
}

BENCHMARK_MAIN()
```

Report fragment is the following:
```
===============================================================================
Benchmark: std::sort
Attempts: 5
Operations: 1
-------------------------------------------------------------------------------
Phase: std::sort(1000000)
Total time: 66.976 ms
Total items: 1000000
Items throughput: 14930626 ops/s
-------------------------------------------------------------------------------
Phase: std::sort(10000000)
Total time: 644.141 ms
Total items: 10000000
Items throughput: 15524528 ops/s
===============================================================================
```

## Example 6: Benchmark class
You can also create a benchmark by inheriting from CppBenchmark::Benchmark class
and implementing Run() method. You can use AddItems() method of a benchmark context
metrics to register processed items.

```c++
#include "cppbenchmark.h"

#include <algorithm>
#include <vector>

class StdSort : public CppBenchmark::Benchmark
{
public:
    using Benchmark::Benchmark;

protected:
    std::vector<int> items;

    void Initialize(CppBenchmark::Context& context) override
    {
        items.resize(context.x());
        std::generate(items.begin(), items.end(), rand);
    }

    void Cleanup(CppBenchmark::Context& context) override
    {
        items.clear();
    }

    void Run(CppBenchmark::Context& context) override
    {
        std::sort(items.begin(), items.end());
        context.metrics().AddItems(items.size());
    }
};

BENCHMARK_CLASS(StdSort, "std::sort", Settings().Param(10000000))

BENCHMARK_MAIN()
```

Report fragment is the following:
```
===============================================================================
Benchmark: std::sort
Attempts: 5
Operations: 1
-------------------------------------------------------------------------------
Phase: std::sort(10000000)
Total time: 648.461 ms
Total items: 10000000
Items throughput: 15421124 ops/s
===============================================================================
```

## Example 7: Benchmark I/O operations
You can use AddBytes() method of a benchmark context metrics to register processed data.

```c++
#include "cppbenchmark.h"

#include <array>

const int operations = 100000;
const int chunk_size_from = 32;
const int chunk_size_to = 4096;

// Create settings for the benchmark which will make 100000 operations for each chunk size
// scaled from 32 bytes to 4096 bytes (32, 64, 128, 256, 512, 1024, 2048, 4096).
const auto settings = CppBenchmark::Settings()
    .Operations(operations)
    .ParamRange(
        chunk_size_from, chunk_size_to, [](int from, int to, int& result)
        {
            int r = result;
            result *= 2;
            return r;
        }
    );

class FileFixture
{
public:
    FileFixture()
    {
        // Open file for binary write
        file = fopen("fwrite.out", "wb");
    }

    ~FileFixture()
    {
        // Close file
        fclose(file);

        // Delete file
        remove("fwrite.out");
    }

protected:
    FILE* file;
    std::array<char, chunk_size_to> buffer;
};

BENCHMARK_FIXTURE(FileFixture, "fwrite", settings)
{
    fwrite(buffer.data(), sizeof(char), context.x(), file);
    context.metrics().AddBytes(context.x());
}

BENCHMARK_MAIN()
```

Report fragment is the following:
```
===============================================================================
Benchmark: fwrite
Attempts: 5
Operations: 100000
-------------------------------------------------------------------------------
Phase: fwrite(32)
Average time: 66 ns/op
Minimal time: 66 ns/op
Maximal time: 78 ns/op
Total time: 6.608 ms
Total operations: 100000
Total bytes: 3.053 MiB
Operations throughput: 15131818 ops/s
Bytes throughput: 461.805 MiB/s
-------------------------------------------------------------------------------
Phase: fwrite(64)
Average time: 93 ns/op
Minimal time: 93 ns/op
Maximal time: 134 ns/op
Total time: 9.380 ms
Total operations: 100000
Total bytes: 6.106 MiB
Operations throughput: 10660950 ops/s
Bytes throughput: 650.709 MiB/s
-------------------------------------------------------------------------------
...
-------------------------------------------------------------------------------
Phase: fwrite(2048)
Average time: 1.846 mcs/op
Minimal time: 1.846 mcs/op
Maximal time: 2.299 mcs/op
Total time: 184.605 ms
Total operations: 100000
Total bytes: 195.320 MiB
Operations throughput: 541696 ops/s
Bytes throughput: 1.034 GiB/s
-------------------------------------------------------------------------------
Phase: fwrite(4096)
Average time: 3.687 mcs/op
Minimal time: 3.687 mcs/op
Maximal time: 4.617 mcs/op
Total time: 368.788 ms
Total operations: 100000
Total bytes: 390.640 MiB
Operations throughput: 271157 ops/s
Bytes throughput: 1.035 GiB/s
===============================================================================
```

## Example 8: Benchmark latency with auto update
```c++
#include "cppbenchmark.h"

#include <chrono>
#include <thread>

const uint64_t operations = 100;
const auto settings = CppBenchmark::Settings().Operations(operations).Latency(1, 1000000000, 5);

BENCHMARK("sleep", settings)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

BENCHMARK_MAIN()
```

Report fragment is the following:
```
===============================================================================
Benchmark: sleep
Attempts: 5
Operations: 100
-------------------------------------------------------------------------------
Phase: sleep
Latency (Min): 10.013 ms/op
Latency (Max): 11.134 ms/op
Latency (Mean): 1.05137e+07
Latency (StDv): 381727
Total time: 1.051 s
Total operations: 100
Operations throughput: 95 ops/s
===============================================================================
```

If the benchmark is launched with **--histograms=100** parameter then a file
with [High Dynamic Range (HDR) Histogram](https://hdrhistogram.github.io/HdrHistogram/)
will be created - [sleep.hdr](https://github.com/chronoxor/CppBenchmark/raw/master/images/sleep.hdr)

Finally you can use [HdrHistogram Plotter](https://hdrhistogram.github.io/HdrHistogram/plotFiles.html)
in order to generate and analyze latency histogram:

![Sleep HDR Histogram](https://github.com/chronoxor/CppBenchmark/raw/master/images/sleep.png)

## Example 9: Benchmark latency with manual update
```c++
#include "benchmark/cppbenchmark.h"

#include <chrono>
#include <limits>

const uint64_t operations = 10000000;
const auto settings = CppBenchmark::Settings().Operations(operations).Latency(1, 1000000000, 5, false);

BENCHMARK("high_resolution_clock", settings)
{
    static uint64_t minresolution = std::numeric_limits<uint64_t>::max();
    static uint64_t maxresolution = std::numeric_limits<uint64_t>::min();
    static auto latency_timestamp = std::chrono::high_resolution_clock::now();
    static auto resolution_timestamp = std::chrono::high_resolution_clock::now();
    static uint64_t count = 0;

    // Get the current timestamp
    auto current = std::chrono::high_resolution_clock::now();

    // Update operations counter
    ++count;

    // Register latency metrics
    uint64_t latency = std::chrono::duration_cast<std::chrono::nanoseconds>(current - latency_timestamp).count();
    if (latency > 0)
    {
        context.metrics().AddLatency(latency / count);
        latency_timestamp = current;
        count = 0;
    }

    // Register resolution metrics
    uint64_t resolution = std::chrono::duration_cast<std::chrono::nanoseconds>(current - resolution_timestamp).count();
    if (resolution > 0)
    {
        if (resolution < minresolution)
        {
            minresolution = resolution;
            context.metrics().SetCustom("resolution-min", minresolution);
        }
        if (resolution > maxresolution)
        {
            maxresolution = resolution;
            context.metrics().SetCustom("resolution-max", maxresolution);
        }
        resolution_timestamp = current;
    }
}
```

Report fragment is the following:
```
===============================================================================
Benchmark: high_resolution_clock
Attempts: 5
Operations: 10000000
-------------------------------------------------------------------------------
Phase: high_resolution_clock
Latency (Min): 38 ns/op
Latency (Max): 1.037 ms/op
Latency (Mean): 53.0462
Latency (StDv): 1136.37
Total time: 468.924 ms
Total operations: 10000000
Operations throughput: 21325385 ops/s
Custom values:
	resolution-max: 7262968
	resolution-min: 311
===============================================================================
```

If the benchmark is launched with **--histograms=100** parameter then a file
with [High Dynamic Range (HDR) Histogram](https://hdrhistogram.github.io/HdrHistogram/)
will be created - [clock.hdr](https://github.com/chronoxor/CppBenchmark/raw/master/images/clock.hdr)

Finally you can use [HdrHistogram Plotter](https://hdrhistogram.github.io/HdrHistogram/plotFiles.html)
in order to generate and analyze latency histogram:

![High resolution clock HDR Histogram](https://github.com/chronoxor/CppBenchmark/raw/master/images/clock.png)

## Example 10: Benchmark threads
```c++
#include "cppbenchmark.h"

#include <atomic>

const int operations = 10000000;

// Create settings for the benchmark which will make 10000000 operations for each
// set of threads scaled from 1 thread to 8 threads (1, 2, 4, 8).
const auto settings = CppBenchmark::Settings()
    .Operations(operations)
    .ThreadsRange(
        1, 8, [](int from, int to, int& result)
        {
            int r = result;
            result *= 2;
            return r;
        }
    );

BENCHMARK_THREADS("std::atomic++", settings)
{
    static std::atomic<int> counter = 0;
    counter++;
}

BENCHMARK_MAIN()
```

Report fragment is the following:
```
===============================================================================
Benchmark: std::atomic++
Attempts: 5
Operations: 10000000
-------------------------------------------------------------------------------
Phase: std::atomic++(threads:1)
Total time: 63.254 ms
-------------------------------------------------------------------------------
Phase: std::atomic++(threads:1).thread
Average time: 6 ns/op
Minimal time: 6 ns/op
Maximal time: 7 ns/op
Total time: 62.809 ms
Total operations: 10000000
Operations throughput: 159210567 ops/s
-------------------------------------------------------------------------------
Phase: std::atomic++(threads:2)
Total time: 362.933 ms
-------------------------------------------------------------------------------
Phase: std::atomic++(threads:2).thread
Average time: 36 ns/op
Minimal time: 36 ns/op
Maximal time: 53 ns/op
Total time: 361.762 ms
Total operations: 10000000
Operations throughput: 27642410 ops/s
-------------------------------------------------------------------------------
Phase: std::atomic++(threads:4)
Total time: 927.259 ms
-------------------------------------------------------------------------------
Phase: std::atomic++(threads:4).thread
Average time: 89 ns/op
Minimal time: 89 ns/op
Maximal time: 94 ns/op
Total time: 898.358 ms
Total operations: 10000000
Operations throughput: 11131419 ops/s
-------------------------------------------------------------------------------
Phase: std::atomic++(threads:8)
Total time: 1.723 s
-------------------------------------------------------------------------------
Phase: std::atomic++(threads:8).thread
Average time: 156 ns/op
Minimal time: 156 ns/op
Maximal time: 173 ns/op
Total time: 1.563 s
Total operations: 10000000
Operations throughput: 6396501 ops/s
===============================================================================
```

## Example 11: Benchmark threads with fixture
```c++
#include "cppbenchmark.h"

#include <array>
#include <atomic>

const int operations = 10000000;

// Create settings for the benchmark which will make 10000000 operations for each
// set of threads scaled from 1 thread to 8 threads (1, 2, 4, 8).
const auto settings = CppBenchmark::Settings()
    .Operations(operations)
    .ThreadsRange(
        1, 8, [](int from, int to, int& result)
        {
            int r = result;
            result *= 2;
            return r;
        }
    );

class Fixture1
{
protected:
    std::atomic<int> counter;
};

class Fixture2 : public virtual CppBenchmark::FixtureThreads
{
protected:
    std::array<int, 8> counter;

    void InitializeThread(CppBenchmark::ContextThread& context) override
    {
        counter[CppBenchmark::System::CurrentThreadId() % counter.size()] = 0;
    }

    void CleanupThread(CppBenchmark::ContextThread& context) override
    {
        // Thread cleanup code can be placed here...
    }
};

BENCHMARK_THREADS_FIXTURE(Fixture1, "Global counter", settings)
{
    counter++;
}

BENCHMARK_THREADS_FIXTURE(Fixture2, "Thread local counter", settings)
{
    counter[CppBenchmark::System::CurrentThreadId() % counter.size()]++;
}

BENCHMARK_MAIN()
```

Report fragment is the following:
```
===============================================================================
Phase: Global counter(threads:1)
Total time: 63.303 ms
-------------------------------------------------------------------------------
Phase: Global counter(threads:1).thread
Average time: 6 ns/op
Minimal time: 6 ns/op
Maximal time: 6 ns/op
Total time: 63.165 ms
Total operations: 10000000
Operations throughput: 158313134 ops/s
-------------------------------------------------------------------------------
Phase: Global counter(threads:2)
Total time: 237.676 ms
-------------------------------------------------------------------------------
Phase: Global counter(threads:2).thread
Average time: 23 ns/op
Minimal time: 23 ns/op
Maximal time: 27 ns/op
Total time: 236.504 ms
Total operations: 10000000
Operations throughput: 42282550 ops/s
-------------------------------------------------------------------------------
Phase: Global counter(threads:4)
Total time: 669.396 ms
-------------------------------------------------------------------------------
Phase: Global counter(threads:4).thread
Average time: 65 ns/op
Minimal time: 65 ns/op
Maximal time: 68 ns/op
Total time: 652.646 ms
Total operations: 10000000
Operations throughput: 15322241 ops/s
-------------------------------------------------------------------------------
Phase: Global counter(threads:8)
Total time: 1.362 s
-------------------------------------------------------------------------------
Phase: Global counter(threads:8).thread
Average time: 129 ns/op
Minimal time: 129 ns/op
Maximal time: 138 ns/op
Total time: 1.297 s
Total operations: 10000000
Operations throughput: 7706848 ops/s
===============================================================================
Phase: Thread local counter(threads:1)
Total time: 41.325 ms
-------------------------------------------------------------------------------
Phase: Thread local counter(threads:1).thread
Average time: 4 ns/op
Minimal time: 4 ns/op
Maximal time: 4 ns/op
Total time: 41.175 ms
Total operations: 10000000
Operations throughput: 242865244 ops/s
-------------------------------------------------------------------------------
Phase: Thread local counter(threads:2)
Total time: 83.360 ms
-------------------------------------------------------------------------------
Phase: Thread local counter(threads:2).thread
Average time: 8 ns/op
Minimal time: 8 ns/op
Maximal time: 12 ns/op
Total time: 80.003 ms
Total operations: 10000000
Operations throughput: 124994748 ops/s
-------------------------------------------------------------------------------
Phase: Thread local counter(threads:4)
Total time: 231.836 ms
-------------------------------------------------------------------------------
Phase: Thread local counter(threads:4).thread
Average time: 21 ns/op
Minimal time: 21 ns/op
Maximal time: 23 ns/op
Total time: 218.354 ms
Total operations: 10000000
Operations throughput: 45797028 ops/s
-------------------------------------------------------------------------------
Phase: Thread local counter(threads:8)
Total time: 412.714 ms
-------------------------------------------------------------------------------
Phase: Thread local counter(threads:8).thread
Average time: 30 ns/op
Minimal time: 30 ns/op
Maximal time: 42 ns/op
Total time: 306.252 ms
Total operations: 10000000
Operations throughput: 32652780 ops/s
===============================================================================
```

## Example 12: Benchmark single producer, single consumer pattern
```c++
#include "cppbenchmark.h"

#include <mutex>
#include <queue>

const int items_to_produce = 10000000;

// Create settings for the benchmark which will create 1 producer and 1 consumer
// and launch producer in inifinite loop.
const auto settings = CppBenchmark::Settings().Infinite().PC(1, 1);

class MutexQueueBenchmark : public CppBenchmark::BenchmarkPC
{
public:
    using BenchmarkPC::BenchmarkPC;

protected:
    void Initialize(CppBenchmark::Context& context) override
    {
        _queue = std::queue<int>();
        _count = 0;
    }

    void Cleanup(CppBenchmark::Context& context) override
    {
        // Benchmark cleanup code can be placed here...
    }

    void InitializeProducer(CppBenchmark::ContextPC& context) override
    {
        // Producer initialize code can be placed here...
    }

    void CleanupProducer(CppBenchmark::ContextPC& context) override
    {
        // Producer cleanup code can be placed here...
    }

    void InitializeConsumer(CppBenchmark::ContextPC& context) override
    {
        // Consumer initialize code can be placed here...
    }

    void CleanupConsumer(CppBenchmark::ContextPC& context) override
    {
        // Consumer cleanup code can be placed here...
    }

    void RunProducer(CppBenchmark::ContextPC& context) override
    {
    	std::lock_guard<std::mutex> lock(_mutex);

        // Check if we need to stop production...
        if (_count >= items_to_produce) {
            _queue.push(0);
            context.StopProduce();
            return;
        }

        // Produce item
        _queue.push(++_count);
    }

    void RunConsumer(CppBenchmark::ContextPC& context) override
    {
    	std::lock_guard<std::mutex> lock(_mutex);

    	if (_queue.size() > 0) {
            // Consume item
            int value = _queue.front();
            _queue.pop();
            // Check if we need to stop consumption...
            if (value == 0) {
                context.StopConsume();
                return;
            }
        }
    }

private:
    std::mutex _mutex;
    std::queue<int> _queue;
    int _count;
};

BENCHMARK_CLASS(MutexQueueBenchmark, "std::mutex+std::queue<int>", settings)

BENCHMARK_MAIN()
```

Report fragment is the following:
```
===============================================================================
Benchmark: std::mutex+std::queue<int>
Attempts: 5
-------------------------------------------------------------------------------
Phase: std::mutex+std::queue<int>(producers:1,consumers:1)
Total time: 652.176 ms
-------------------------------------------------------------------------------
Phase: std::mutex+std::queue<int>(producers:1,consumers:1).producer
Average time: 50 ns/op
Minimal time: 50 ns/op
Maximal time: 53 ns/op
Total time: 509.201 ms
Total operations: 10000001
Operations throughput: 19638574 ops/s
-------------------------------------------------------------------------------
Phase: std::mutex+std::queue<int>(producers:1,consumers:1).consumer
Average time: 64 ns/op
Minimal time: 64 ns/op
Maximal time: 67 ns/op
Total time: 650.805 ms
Total operations: 10124742
Operations throughput: 15557246 ops/s
===============================================================================
```

## Example 13: Benchmark multiple producers, multiple consumers pattern
```c++
#include "cppbenchmark.h"

#include <mutex>
#include <queue>

const int items_to_produce = 10000000;

// Create settings for the benchmark which will create 1/2/4/8 producers and 1/2/4/8 consumers
// and launch all producers in inifinite loop.
const auto settings = CppBenchmark::Settings()
    .Infinite()
    .PCRange(
        1, 8, [](int producers_from, int producers_to, int& producers_result)
        {
            int r = producers_result;
            producers_result *= 2;
            return r;
        },
        1, 8, [](int consumers_from, int consumers_to, int& consumers_result)
        {
            int r = consumers_result;
            consumers_result *= 2;
            return r;
        }
    );

class MutexQueueBenchmark : public CppBenchmark::BenchmarkPC
{
public:
    using BenchmarkPC::BenchmarkPC;

protected:
    void Initialize(CppBenchmark::Context& context) override
    {
        _queue = std::queue<int>();
        _count = 0;
    }

    void Cleanup(CppBenchmark::Context& context) override
    {
        // Benchmark cleanup code can be placed here...
    }

    void InitializeProducer(CppBenchmark::ContextPC& context) override
    {
        // Producer initialize code can be placed here...
    }

    void CleanupProducer(CppBenchmark::ContextPC& context) override
    {
        // Producer cleanup code can be placed here...
    }

    void InitializeConsumer(CppBenchmark::ContextPC& context) override
    {
        // Consumer initialize code can be placed here...
    }

    void CleanupConsumer(CppBenchmark::ContextPC& context) override
    {
        // Consumer cleanup code can be placed here...
    }

    void RunProducer(CppBenchmark::ContextPC& context) override
    {
    	std::lock_guard<std::mutex> lock(_mutex);

        // Check if we need to stop production...
        if (_count >= items_to_produce) {
            _queue.push(0);
            context.StopProduce();
            return;
        }

        // Produce item
        _queue.push(++_count);
    }

    void RunConsumer(CppBenchmark::ContextPC& context) override
    {
    	std::lock_guard<std::mutex> lock(_mutex);

    	if (_queue.size() > 0) {
            // Consume item
            int value = _queue.front();
            _queue.pop();
            // Check if we need to stop consumption...
            if (value == 0) {
                context.StopConsume();
                return;
            }
        }
    }

private:
    std::mutex _mutex;
    std::queue<int> _queue;
    int _count;
};

BENCHMARK_CLASS(MutexQueueBenchmark, "std::mutex+std::queue<int>", settings)

BENCHMARK_MAIN()
```

Report fragment is the following:
```
===============================================================================
Benchmark: std::mutex+std::queue<int>
Attempts: 5
-------------------------------------------------------------------------------
Phase: std::mutex+std::queue<int>(producers:1,consumers:1)
Total time: 681.430 ms
-------------------------------------------------------------------------------
Phase: std::mutex+std::queue<int>(producers:1,consumers:1).producer
Average time: 42 ns/op
Minimal time: 42 ns/op
Maximal time: 120 ns/op
Total time: 427.075 ms
Total operations: 10000001
Operations throughput: 23415052 ops/s
-------------------------------------------------------------------------------
Phase: std::mutex+std::queue<int>(producers:1,consumers:1).consumer
Average time: 67 ns/op
Minimal time: 67 ns/op
Maximal time: 120 ns/op
Total time: 679.235 ms
Total operations: 10000001
Operations throughput: 14722437 ops/s
-------------------------------------------------------------------------------
Phase: std::mutex+std::queue<int>(producers:1,consumers:2)
Total time: 623.887 ms
-------------------------------------------------------------------------------
Phase: std::mutex+std::queue<int>(producers:1,consumers:2).producer
Average time: 58 ns/op
Minimal time: 58 ns/op
Maximal time: 103 ns/op
Total time: 582.786 ms
Total operations: 10000001
Operations throughput: 17158941 ops/s
-------------------------------------------------------------------------------
Phase: std::mutex+std::queue<int>(producers:1,consumers:2).consumer
Average time: 125 ns/op
Minimal time: 125 ns/op
Maximal time: 208 ns/op
Total time: 622.654 ms
Total operations: 4963799
Operations throughput: 7971989 ops/s
-------------------------------------------------------------------------------
...
-------------------------------------------------------------------------------
Phase: std::mutex+std::queue<int>(producers:8,consumers:4)
Total time: 820.237 ms
-------------------------------------------------------------------------------
Phase: std::mutex+std::queue<int>(producers:8,consumers:4).producer
Average time: 835 ns/op
Minimal time: 835 ns/op
Maximal time: 1.032 mcs/op
Total time: 606.745 ms
Total operations: 725823
Operations throughput: 1196256 ops/s
-------------------------------------------------------------------------------
Phase: std::mutex+std::queue<int>(producers:8,consumers:4).consumer
Average time: 213 ns/op
Minimal time: 213 ns/op
Maximal time: 264 ns/op
Total time: 755.649 ms
Total operations: 3543116
Operations throughput: 4688834 ops/s
-------------------------------------------------------------------------------
Phase: std::mutex+std::queue<int>(producers:8,consumers:8)
Total time: 824.811 ms
-------------------------------------------------------------------------------
Phase: std::mutex+std::queue<int>(producers:8,consumers:8).producer
Average time: 485 ns/op
Minimal time: 485 ns/op
Maximal time: 565 ns/op
Total time: 743.897 ms
Total operations: 1533043
Operations throughput: 2060824 ops/s
-------------------------------------------------------------------------------
Phase: std::mutex+std::queue<int>(producers:8,consumers:8).consumer
Average time: 489 ns/op
Minimal time: 489 ns/op
Maximal time: 648 ns/op
Total time: 676.364 ms
Total operations: 1382941
Operations throughput: 2044668 ops/s
===============================================================================
```

## Example 14: Dynamic benchmarks
Dynamic benchmarks are usefull when you have some working program and want to benchmark some
critical parts and code fragments. In this case just include cppbenchmark.h header and use
BENCHCODE_SCOPE(), BENCHCODE_START(), BENCHCODE_STOP(), BENCHCODE_REPORT() macro. All of the
macro are easy access to methods of the static [Executor](http://chronoxor.github.io/CppBenchmark/class_cpp_benchmark_1_1_executor.html) class
which you may use directly as a singleton. All functionality provided for dynamic benchmarks is
thread-safe synchronizied with mutex (each call will lose some ns).

```c++
#include "cppbenchmark.h"

#include <chrono>
#include <thread>
#include <vector>

const int THREADS = 8;

void init()
{
    auto benchmark = BENCHCODE_SCOPE("Initialization");

    std::this_thread::sleep_for(std::chrono::seconds(2));
}

void calculate()
{
    auto benchmark = BENCHCODE_SCOPE("Calculate");

    for (int i = 0; i < 5; ++i) {
        auto phase1 = benchmark->StartPhase("Calculate.1");

        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        phase1->StopPhase();
    }

    auto phase2 = benchmark->StartPhase("Calculate.2");
    {
        auto phase21 = benchmark->StartPhase("Calculate.2.1");

        std::this_thread::sleep_for(std::chrono::milliseconds(200));

        phase21->StopPhase();

        auto phase22 = benchmark->StartPhase("Calculate.2.2");

        std::this_thread::sleep_for(std::chrono::milliseconds(300));

        phase22->StopPhase();
    }
    phase2->StopPhase();

    for (int i = 0; i < 3; ++i) {
        auto phase3 = benchmark->StartPhase("Calculate.3");

        std::this_thread::sleep_for(std::chrono::milliseconds(400));

        phase3->StopPhase();
    }
}

void cleanup()
{
    BENCHCODE_START("Cleanup");

    std::this_thread::sleep_for(std::chrono::seconds(1));

    BENCHCODE_STOP("Cleanup");
}

int main(int argc, char** argv)
{
    // Initialization
    init();

    // Start parallel calculations
    std::vector<std::thread> threads;
    for (int i = 0; i < THREADS; ++i)
        threads.push_back(std::thread(calculate));

    // Wait for all threads
    for (auto& thread : threads)
        thread.join();

    // Cleanup
    cleanup();

    // Report benchmark results
    BENCHCODE_REPORT();

    return 0;
}
```

Report fragment is the following:
```
===============================================================================
Benchmark: Initialization
Attempts: 1
Operations: 1
-------------------------------------------------------------------------------
Phase: Initialization
Total time: 2.002 s
===============================================================================
Benchmark: Calculate
Attempts: 1
Operations: 1
-------------------------------------------------------------------------------
Phase: Calculate
Total time: 2.200 s
-------------------------------------------------------------------------------
Phase: Calculate.1
Average time: 100.113 ms/op
Minimal time: 93.337 ms/op
Maximal time: 107.303 ms/op
Total time: 500.565 ms
Total operations: 5
Operations throughput: 9 ops/s
-------------------------------------------------------------------------------
Phase: Calculate.2
Total time: 499.420 ms
-------------------------------------------------------------------------------
Phase: Calculate.2.1
Total time: 199.514 ms
-------------------------------------------------------------------------------
Phase: Calculate.2.2
Total time: 299.755 ms
-------------------------------------------------------------------------------
Phase: Calculate.3
Average time: 399.920 ms/op
Minimal time: 399.726 ms/op
Maximal time: 400.365 ms/op
Total time: 1.199 s
Total operations: 3
Operations throughput: 2 ops/s
===============================================================================
Benchmark: Cleanup
Attempts: 1
Operations: 1
-------------------------------------------------------------------------------
Phase: Cleanup
Total time: 1.007 s
===============================================================================
```

# Command line options
When you create and build a benchmark you can run it with the following command line options:
* **--version**  - Show program's version number and exit
* **-h, --help** - Show this help message and exit
* **-f FILTER, --filter=FILTER** - Filter benchmarks by the given regexp pattern
* **-l, --list** - List all avaliable benchmarks
* **-o OUTPUT, --output=OUTPUT** - Output format (console, csv, json). Default: console
* **-s, --silent** - Launch in silent mode. No progress will be shown!
* **-r HISTOGRAMS, --histograms=HISTOGRAMS** - Create High Dynamic Range (HDR) Histogram files with a given resolution. Default: 0
