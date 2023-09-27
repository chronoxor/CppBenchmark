# CppBenchmark

[![License](https://img.shields.io/badge/License-MIT-green.svg)](LICENSE)
[![Release](https://img.shields.io/github/release/chronoxor/CppBenchmark.svg?sort=semver)](https://github.com/chronoxor/CppBenchmark/releases)
<br/>
[![Linux (clang)](https://github.com/chronoxor/CppBenchmark/actions/workflows/build-linux-clang.yml/badge.svg)](https://github.com/chronoxor/CppBenchmark/actions/workflows/build-linux-clang.yml)
[![Linux (gcc)](https://github.com/chronoxor/CppBenchmark/actions/workflows/build-linux-gcc.yml/badge.svg)](https://github.com/chronoxor/CppBenchmark/actions/workflows/build-linux-gcc.yml)
[![MacOS](https://github.com/chronoxor/CppBenchmark/actions/workflows/build-macos.yml/badge.svg)](https://github.com/chronoxor/CppBenchmark/actions/workflows/build-macos.yml)
<br/>
[![Windows (Cygwin)](https://github.com/chronoxor/CppBenchmark/actions/workflows/build-windows-cygwin.yml/badge.svg)](https://github.com/chronoxor/CppBenchmark/actions/workflows/build-windows-cygwin.yml)
[![Windows (MSYS2)](https://github.com/chronoxor/CppBenchmark/actions/workflows/build-windows-msys2.yml/badge.svg)](https://github.com/chronoxor/CppBenchmark/actions/workflows/build-windows-msys2.yml)
[![Windows (MinGW)](https://github.com/chronoxor/CppBenchmark/actions/workflows/build-windows-mingw.yml/badge.svg)](https://github.com/chronoxor/CppBenchmark/actions/workflows/build-windows-mingw.yml)
[![Windows (Visual Studio)](https://github.com/chronoxor/CppBenchmark/actions/workflows/build-windows-vs.yml/badge.svg)](https://github.com/chronoxor/CppBenchmark/actions/workflows/build-windows-vs.yml)

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
* Cross platform (Linux, MacOS, Windows)
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
* MacOS
* Windows
* [cmake](https://www.cmake.org)
* [gcc](https://gcc.gnu.org)
* [git](https://git-scm.com)
* [gil](https://github.com/chronoxor/gil.git)
* [python3](https://www.python.org)

Optional:
* [clang](https://clang.llvm.org)
* [CLion](https://www.jetbrains.com/clion)
* [Cygwin](https://cygwin.com)
* [MSYS2](https://www.msys2.org)
* [MinGW](https://mingw-w64.org/doku.php)
* [Visual Studio](https://www.visualstudio.com)

# How to build?

### Install [gil (git links) tool](https://github.com/chronoxor/gil)
```shell
pip3 install gil
```

### Setup repository
```shell
git clone https://github.com/chronoxor/CppBenchmark.git
cd CppBenchmark
gil update
```

### Linux
```shell
cd build
./unix.sh
```

### MacOS
```shell
cd build
./unix.sh
```

### Windows (Cygwin)
```shell
cd build
unix.bat
```

### Windows (MSYS2)
```shell
cd build
unix.bat
```

### Windows (MinGW)
```shell
cd build
mingw.bat
```

### Windows (Visual Studio)
```shell
cd build
vs.bat
```

# How to create a benchmark?
1. [Build CppBenchmark library](#how-to-build)
2. Create a new *.cpp file
3. Insert #include "benchmark/cppbenchmark.h"
4. Add benchmark code (examples for different scenarios you can find below)
5. Insert BENCHMARK_MAIN() at the end
6. Compile the *.cpp file and link it with CppBenchmark library
7. Run it (see also possible [command line options](#command-line-options))

# Benchmark examples

## Example 1: Benchmark of a function call
```c++
#include "benchmark/cppbenchmark.h"

#include <math.h>

// Benchmark sin() call for 5 seconds (by default).
// Make 5 attemtps (by default) and choose one with the best time result.
BENCHMARK("sin")
{
    sin(123.456);
}

BENCHMARK_MAIN()
```

Report fragment is the following:
```
===============================================================================
Benchmark: sin()
Attempts: 5
Duration: 5 seconds
-------------------------------------------------------------------------------
Phase: sin()
Average time: 6 ns/op
Minimal time: 6 ns/op
Maximal time: 6 ns/op
Total time: 858.903 ms
Total operations: 130842248
Operations throughput: 152336350 ops/s
===============================================================================
```

## Example 2: Benchmark with cancelation
```c++
#include "benchmark/cppbenchmark.h"

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
Benchmark: rand()-till-zero
Attempts: 10
-------------------------------------------------------------------------------
Phase: rand()-till-zero
Average time: 15 ns/op
Minimal time: 15 ns/op
Maximal time: 92 ns/op
Total time: 159.936 mcs
Total operations: 10493
Operations throughput: 65607492 ops/s
===============================================================================
```

## Example 3: Benchmark with static fixture
Static fixture will be constructed once per each benchmark, will be the same for
each attempt / operation and will be destructed at the end of the benchmark.

```c++
#include "macros.h"

#include <list>
#include <vector>

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

BENCHMARK_FIXTURE(ContainerFixture<std::list<int>>, "std::list<int>.forward")
{
    for (auto it = container.begin(); it != container.end(); ++it)
        ++(*it);
}

BENCHMARK_FIXTURE(ContainerFixture<std::list<int>>, "std::list<int>.backward")
{
    for (auto it = container.rbegin(); it != container.rend(); ++it)
        ++(*it);
}

BENCHMARK_FIXTURE(ContainerFixture<std::vector<int>>, "std::vector<int>.forward")
{
    for (auto it = container.begin(); it != container.end(); ++it)
        ++(*it);
}

BENCHMARK_FIXTURE(ContainerFixture<std::vector<int>>, "std::vector<int>.backward")
{
    for (auto it = container.rbegin(); it != container.rend(); ++it)
        ++(*it);
}

BENCHMARK_MAIN()
```

Report fragment is the following:
```
===============================================================================
Benchmark: std::list<int>-forward
Attempts: 5
Duration: 5 seconds
-------------------------------------------------------------------------------
Phase: std::list<int>-forward
Average time: 6.332 ms/op
Minimal time: 6.332 ms/op
Maximal time: 6.998 ms/op
Total time: 4.958 s
Total operations: 783
Operations throughput: 157 ops/s
===============================================================================
Benchmark: std::list<int>-backward
Attempts: 5
Duration: 5 seconds
-------------------------------------------------------------------------------
Phase: std::list<int>-backward
Average time: 7.883 ms/op
Minimal time: 7.883 ms/op
Maximal time: 8.196 ms/op
Total time: 4.911 s
Total operations: 623
Operations throughput: 126 ops/s
===============================================================================
Benchmark: std::vector<int>-forward
Attempts: 5
Duration: 5 seconds
-------------------------------------------------------------------------------
Phase: std::vector<int>-forward
Average time: 298.114 mcs/op
Minimal time: 298.114 mcs/op
Maximal time: 308.209 mcs/op
Total time: 4.852 s
Total operations: 16276
Operations throughput: 3354 ops/s
===============================================================================
Benchmark: std::vector<int>-backward
Attempts: 5
Duration: 5 seconds
-------------------------------------------------------------------------------
Phase: std::vector<int>-backward
Average time: 316.412 mcs/op
Minimal time: 316.412 mcs/op
Maximal time: 350.224 mcs/op
Total time: 4.869 s
Total operations: 15390
Operations throughput: 3160 ops/s
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

template <typename T>
class ContainerFixture : public virtual CppBenchmark::Fixture
{
protected:
    T container;

    void Initialize(CppBenchmark::Context& context) override { container = T(); }
    void Cleanup(CppBenchmark::Context& context) override { container.clear(); }
};

BENCHMARK_FIXTURE(ContainerFixture<std::list<int>>, "std::list<int>.push_back")
{
    container.push_back(0);
}

BENCHMARK_FIXTURE(ContainerFixture<std::vector<int>>, "std::vector<int>.push_back")
{
    container.push_back(0);
}

BENCHMARK_FIXTURE(ContainerFixture<std::deque<int>>, "std::deque<int>.push_back")
{
    container.push_back(0);
}

BENCHMARK_MAIN()
```

Report fragment is the following:
```
===============================================================================
Benchmark: std::list<int>.push_back()
Attempts: 5
Duration: 5 seconds
-------------------------------------------------------------------------------
Phase: std::list<int>.push_back()
Average time: 35 ns/op
Minimal time: 35 ns/op
Maximal time: 39 ns/op
Total time: 2.720 s
Total operations: 76213307
Operations throughput: 28009633 ops/s
===============================================================================
Benchmark: std::vector<int>.push_back()
Attempts: 5
Duration: 5 seconds
-------------------------------------------------------------------------------
Phase: std::vector<int>.push_back()
Average time: 5 ns/op
Minimal time: 5 ns/op
Maximal time: 5 ns/op
Total time: 722.837 ms
Total operations: 126890166
Operations throughput: 175544557 ops/s
===============================================================================
Benchmark: std::deque<int>.push_back()
Attempts: 5
Duration: 5 seconds
-------------------------------------------------------------------------------
Phase: std::deque<int>.push_back()
Average time: 12 ns/op
Minimal time: 12 ns/op
Maximal time: 12 ns/op
Total time: 1.319 s
Total operations: 105369784
Operations throughput: 79858488 ops/s
===============================================================================
```

## Example 5: Benchmark with parameters
Additional parameters can be provided to benchmark with settings using fluent
syntax. Parameters can be single, pair or tripple, provided as a value, as a
range, or with a range and selector function. Benchmark will be launched for
each parameters combination.

```c++
#include "benchmark/cppbenchmark.h"

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
#include "benchmark/cppbenchmark.h"

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
#include "benchmark/cppbenchmark.h"

#include <array>

const int chunk_size_from = 32;
const int chunk_size_to = 4096;

// Create settings for the benchmark which will launch for each chunk size
// scaled from 32 bytes to 4096 bytes (32, 64, 128, 256, 512, 1024, 2048, 4096).
const auto settings = CppBenchmark::Settings()
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
Benchmark: fwrite()
Attempts: 5
Duration: 5 seconds
-------------------------------------------------------------------------------
Phase: fwrite()(32)
Average time: 55 ns/op
Minimal time: 55 ns/op
Maximal time: 108 ns/op
Total time: 2.821 s
Total operations: 50703513
Total bytes: 1.523 GiB
Operations throughput: 17968501 ops/s
Bytes throughput: 548.363 MiB/s
-------------------------------------------------------------------------------
Phase: fwrite()(64)
Average time: 93 ns/op
Minimal time: 93 ns/op
Maximal time: 162 ns/op
Total time: 3.820 s
Total operations: 40744084
Total bytes: 2.438 GiB
Operations throughput: 10665202 ops/s
Bytes throughput: 650.975 MiB/s
-------------------------------------------------------------------------------
...
-------------------------------------------------------------------------------
Phase: fwrite()(2048)
Average time: 8.805 mcs/op
Minimal time: 8.805 mcs/op
Maximal time: 11.895 mcs/op
Total time: 3.968 s
Total operations: 450686
Total bytes: 880.252 MiB
Operations throughput: 113569 ops/s
Bytes throughput: 221.835 MiB/s
-------------------------------------------------------------------------------
Phase: fwrite()(4096)
Average time: 19.485 mcs/op
Minimal time: 19.485 mcs/op
Maximal time: 20.887 mcs/op
Total time: 4.906 s
Total operations: 251821
Total bytes: 983.692 MiB
Operations throughput: 51319 ops/s
Bytes throughput: 200.478 MiB/s
===============================================================================
```

## Example 8: Benchmark latency with auto update
```c++
#include "benchmark/cppbenchmark.h"

#include <chrono>
#include <thread>

const auto settings = CppBenchmark::Settings().Latency(1, 1000000000, 5);

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
Duration: 5 seconds
-------------------------------------------------------------------------------
Phase: sleep
Latency (Min): 10.014 ms/op
Latency (Max): 11.377 ms/op
Latency (Mean): 1.04928e+07
Latency (StDv): 364511
Total time: 4.985 s
Total operations: 571
Operations throughput: 114 ops/s
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

const auto settings = CppBenchmark::Settings().Operations(10000000).Latency(1, 1000000000, 5, false);

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
#include "benchmark/cppbenchmark.h"

#include <atomic>

// Create settings for the benchmark which will launch for each
// set of threads scaled from 1 thread to 8 threads (1, 2, 4, 8).
const auto settings = CppBenchmark::Settings()
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
Duration: 5 seconds
-------------------------------------------------------------------------------
Phase: std::atomic++(threads:1)
Average time: 19 ns/op
Minimal time: 19 ns/op
Maximal time: 20 ns/op
Total time: 2.124 s
Total operations: 111355461
Operations throughput: 52425884 ops/s
-------------------------------------------------------------------------------
Phase: std::atomic++(threads:1).thread
Average time: 5 ns/op
Minimal time: 5 ns/op
Maximal time: 5 ns/op
Total time: 586.191 ms
Total operations: 111355461
Operations throughput: 189964343 ops/s
-------------------------------------------------------------------------------
Phase: std::atomic++(threads:2)
Average time: 20 ns/op
Minimal time: 20 ns/op
Maximal time: 24 ns/op
Total time: 3.907 s
Total operations: 188624150
Operations throughput: 48270817 ops/s
-------------------------------------------------------------------------------
Phase: std::atomic++(threads:2).thread
Average time: 23 ns/op
Minimal time: 23 ns/op
Maximal time: 30 ns/op
Total time: 2.179 s
Total operations: 94312075
Operations throughput: 43270119 ops/s
-------------------------------------------------------------------------------
Phase: std::atomic++(threads:4)
Average time: 18 ns/op
Minimal time: 18 ns/op
Maximal time: 19 ns/op
Total time: 6.875 s
Total operations: 365529364
Operations throughput: 53160207 ops/s
-------------------------------------------------------------------------------
Phase: std::atomic++(threads:4).thread
Average time: 56 ns/op
Minimal time: 56 ns/op
Maximal time: 60 ns/op
Total time: 5.142 s
Total operations: 91382341
Operations throughput: 17771705 ops/s
-------------------------------------------------------------------------------
Phase: std::atomic++(threads:8)
Average time: 23 ns/op
Minimal time: 23 ns/op
Maximal time: 25 ns/op
Total time: 7.667 s
Total operations: 330867224
Operations throughput: 43153297 ops/s
-------------------------------------------------------------------------------
Phase: std::atomic++(threads:8).thread
Average time: 105 ns/op
Minimal time: 105 ns/op
Maximal time: 167 ns/op
Total time: 4.367 s
Total operations: 41358403
Operations throughput: 9468527 ops/s
===============================================================================
```

## Example 11: Benchmark threads with fixture
```c++
#include "benchmark/cppbenchmark.h"

#include <array>
#include <atomic>

// Create settings for the benchmark which will launch for each
// set of threads scaled from 1 thread to 8 threads (1, 2, 4, 8).
const auto settings = CppBenchmark::Settings()
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

    void InitializeThread(CppBenchmark::ContextThreads& context) override
    {
        counter[CppBenchmark::System::CurrentThreadId() % counter.size()] = 0;
    }

    void CleanupThread(CppBenchmark::ContextThreads& context) override
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
Benchmark: Global counter
Attempts: 5
Duration: 5 seconds
-------------------------------------------------------------------------------
Phase: Global counter(threads:1).thread
Average time: 5 ns/op
Minimal time: 5 ns/op
Maximal time: 5 ns/op
Total time: 629.639 ms
Total operations: 119518816
Operations throughput: 189821077 ops/s
-------------------------------------------------------------------------------
Phase: Global counter(threads:2).thread
Average time: 18 ns/op
Minimal time: 18 ns/op
Maximal time: 24 ns/op
Total time: 1.860 s
Total operations: 101568823
Operations throughput: 54581734 ops/s
-------------------------------------------------------------------------------
Phase: Global counter(threads:4).thread
Average time: 57 ns/op
Minimal time: 57 ns/op
Maximal time: 66 ns/op
Total time: 4.552 s
Total operations: 79503346
Operations throughput: 17464897 ops/s
-------------------------------------------------------------------------------
Phase: Global counter(threads:8).thread
Average time: 103 ns/op
Minimal time: 103 ns/op
Maximal time: 143 ns/op
Total time: 4.601 s
Total operations: 44597477
Operations throughput: 9690967 ops/s
===============================================================================
Benchmark: Thread local counter
Attempts: 5
Duration: 5 seconds
-------------------------------------------------------------------------------
Phase: Thread local counter(threads:1).thread
Average time: 4 ns/op
Minimal time: 4 ns/op
Maximal time: 4 ns/op
Total time: 739.689 ms
Total operations: 166432112
Operations throughput: 225002770 ops/s
-------------------------------------------------------------------------------
Phase: Thread local counter(threads:2).thread
Average time: 9 ns/op
Minimal time: 9 ns/op
Maximal time: 10 ns/op
Total time: 1.061 s
Total operations: 113102777
Operations throughput: 106564314 ops/s
-------------------------------------------------------------------------------
Phase: Thread local counter(threads:4).thread
Average time: 20 ns/op
Minimal time: 20 ns/op
Maximal time: 21 ns/op
Total time: 1.944 s
Total operations: 94786108
Operations throughput: 48757481 ops/s
-------------------------------------------------------------------------------
Phase: Thread local counter(threads:8).thread
Average time: 25 ns/op
Minimal time: 25 ns/op
Maximal time: 39 ns/op
Total time: 1.784 s
Total operations: 71185751
Operations throughput: 39887088 ops/s
===============================================================================
```

## Example 12: Benchmark single producer, single consumer pattern
```c++
#include "benchmark/cppbenchmark.h"

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
#include "benchmark/cppbenchmark.h"

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
#include "benchmark/cppbenchmark.h"

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
* **-q, --quiet** - Launch in quiet mode. No progress will be shown!
* **-r HISTOGRAMS, --histograms=HISTOGRAMS** - Create High Dynamic Range (HDR) Histogram files with a given resolution. Default: 0
