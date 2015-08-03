# CppBenchmark
C++ Benchmark Library

#Contents
  * [Requirements](#requirements)
  * [How to build?](#how-to-build)
    * [Windows (Visaul Studio 2015)](#windows-visaul-studio-2015)
    * [Windows (MinGW with MSYS)](#windows-mingw-with-msys)
    * [Linux](#linux)
  * [How to create a benchmark?](#how-to-create-a-benchmark)
    * [Example 1: Benchmark of a function call](#example-1-benchmark-of-a-function-call)
    * [Example 2: Benchmark with cancelation](#example-2-benchmark-with-cancelation) 
    * [Example 3: Benchmark with static fixture](#example-3-benchmark-with-static-fixture)
    * [Example 4: Benchmark with dynamic fixture](#example-4-benchmark-with-dynamic-fixture)
    * [Example 5: Benchmark with parameters](#example-5-benchmark-with-parameters)
    * [Example 6: Benchmark class](#example-6-benchmark-class)
    * [Example 7: Benchmark I/O operations](#example-7-benchmark-io-operations)
  * [Command line options](#command-line-options)     
  * [Todo](#todo)

# Requirements
* Windows 7 / Windows 10
* Linux
* [Visual Studio 2015](https://www.visualstudio.com/)
* GCC 5.0.0
* [CMake 3.3.0](http://www.cmake.org/download/)

#How to build?

## Windows (Visaul Studio 2015)
```
git clone https://github.com/chronoxor/CppBenchmark.git
cd CppBenchmark\scripts
01-generate-vs2015-x64.bat
02-build-vs2015.bat
03-tests.bat
04-install-vs2015.bat
05-doxygen-vs2015.bat
```
If you want 32-bit version use '01-generate-vs2015-x32.bat' to generate project files.

## Windows (MinGW with MSYS)
```
git clone https://github.com/chronoxor/CppBenchmark.git
cd CppBenchmark\scripts
01-generate-MSYS.bat
02-build-MSYS.bat
03-tests.bat
04-install-MSYS.bat
05-doxygen-MSYS.bat
```

## Linux
```
git clone https://github.com/chronoxor/CppBenchmark.git
cd CppBenchmark\scripts
01-generate-unix.sh
02-build-unix.sh
03-tests.bat
04-install-unix.sh
05-doxygen-unix.sh
```

#How to create a benchmark?
1. [Build CppBenchmark library](#how-to-build)
2. Create a new *.cpp file
3. Insert #include "cppbenchmark.h"
4. Add benchmark code (examples for different scenarios you can find below)
5. Insert BENCHMARK_MAIN() at the end
6. Compile the *.cpp file and link it over CppBenchmark library
7. Run it (see also possible [command line options](#command-line-options))

##Example 1: Benchmark of a function call
```C++
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
Iterations: 1000000000
-------------------------------------------------------------------------------
Phase: sin()
Average time: 2 ns / iteration
Minimal time: 2 ns / iteration
Maximal time: 2 ns / iteration
Total time: 2.428 s
Total iterations: 1000000000
Iterations throughput: 411732889 / second
===============================================================================
```

##Example 2: Benchmark with cancelation
```C++
#include "cppbenchmark.h"

// Benchmark rand() call until it returns 0. 
// Benchmark will print iterations count required to get 'rand() == 0' case.
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
Average time: 25 ns / iteration
Minimal time: 25 ns / iteration
Maximal time: 513 ns / iteration
Total time: 94.234 mcs
Total iterations: 3716
Iterations throughput: 39433750 / second
===============================================================================
```

##Example 3: Benchmark with static fixture
Static fixture will be constructed once per each benchmark, will be the same for 
each attempt / iteration and will be destructed at the end of the benchmark.
```C++
#include "macros.h"

#include <list>
#include <vector>

const int iterations = 1000;

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

BENCHMARK_FIXTURE(ContainerFixture<std::list<int>>, "std::list<int>.forward", iterations)
{
    for (auto it = container.begin(); it != container.end(); ++it)
        ++(*it);
}

BENCHMARK_FIXTURE(ContainerFixture<std::list<int>>, "std::list<int>.backward", iterations)
{
    for (auto it = container.rbegin(); it != container.rend(); ++it)
        ++(*it);
}

BENCHMARK_FIXTURE(ContainerFixture<std::vector<int>>, "std::vector<int>.forward", iterations)
{
    for (auto it = container.begin(); it != container.end(); ++it)
        ++(*it);
}

BENCHMARK_FIXTURE(ContainerFixture<std::vector<int>>, "std::vector<int>.backward", iterations)
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
Iterations: 1000
-------------------------------------------------------------------------------
Phase: std::list<int>.forward()
Average time: 6.055 ms / iteration
Minimal time: 6.055 ms / iteration
Maximal time: 6.337 ms / iteration
Total time: 6.055 s
Total iterations: 1000
Iterations throughput: 165 / second
===============================================================================
Benchmark: std::list<int>.backward
Attempts: 5
Iterations: 1000
-------------------------------------------------------------------------------
Phase: std::list<int>.backward()
Average time: 6.075 ms / iteration
Minimal time: 6.075 ms / iteration
Maximal time: 6.935 ms / iteration
Total time: 6.075 s
Total iterations: 1000
Iterations throughput: 164 / second
===============================================================================
Benchmark: std::vector<int>.forward
Attempts: 5
Iterations: 1000
-------------------------------------------------------------------------------
Phase: std::vector<int>.forward()
Average time: 663.003 mcs / iteration
Minimal time: 663.003 mcs / iteration
Maximal time: 678.439 mcs / iteration
Total time: 663.003 ms
Total iterations: 1000
Iterations throughput: 1508 / second
===============================================================================
Benchmark: std::vector<int>.backward
Attempts: 5
Iterations: 1000
-------------------------------------------------------------------------------
Phase: std::vector<int>.backward()
Average time: 667.515 mcs / iteration
Minimal time: 667.515 mcs / iteration
Maximal time: 681.929 mcs / iteration
Total time: 667.515 ms
Total iterations: 1000
Iterations throughput: 1498 / second
===============================================================================
```

##Example 4: Benchmark with dynamic fixture
Dynamic fixture can be used to prepare benchmark before each attempt with 
Initialize() / Cleanup() methods. You can access to the current benchmark
context in dynamic fixture methods.
```C++
#include "macros.h"

#include <deque>
#include <list>
#include <vector>

const int iterations = 10000000;

template <typename T>
class ContainerFixture : public virtual CppBenchmark::Fixture
{
protected:
    T container;

    void Initialize(CppBenchmark::Context& context) override { container = T(); }
    void Cleanup(CppBenchmark::Context& context) override { container.clear(); }
};

BENCHMARK_FIXTURE(ContainerFixture<std::list<int>>, "std::list<int>.push_back", iterations)
{
    container.push_back(0);
}

BENCHMARK_FIXTURE(ContainerFixture<std::vector<int>>, "std::vector<int>.push_back", iterations)
{
    container.push_back(0);
}

BENCHMARK_FIXTURE(ContainerFixture<std::deque<int>>, "std::deque<int>.push_back", iterations)
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
Iterations: 10000000
-------------------------------------------------------------------------------
Phase: std::list<int>.push_back()
Average time: 50 ns / iteration
Minimal time: 50 ns / iteration
Maximal time: 53 ns / iteration
Total time: 505.169 ms
Total iterations: 10000000
Iterations throughput: 19795348 / second
===============================================================================
Benchmark: std::vector<int>.push_back
Attempts: 5
Iterations: 10000000
-------------------------------------------------------------------------------
Phase: std::vector<int>.push_back()
Average time: 9 ns / iteration
Minimal time: 9 ns / iteration
Maximal time: 10 ns / iteration
Total time: 97.252 ms
Total iterations: 10000000
Iterations throughput: 102824688 / second
===============================================================================
Benchmark: std::deque<int>.push_back
Attempts: 5
Iterations: 10000000
-------------------------------------------------------------------------------
Phase: std::deque<int>.push_back()
Average time: 21 ns / iteration
Minimal time: 21 ns / iteration
Maximal time: 22 ns / iteration
Total time: 218.826 ms
Total iterations: 10000000
Iterations throughput: 45698221 / second
===============================================================================
```

##Example 5: Benchmark with parameters
Additional parameters can be provided to benchmark with settings using fluent 
syntax. Parameters can be single, pair or tripple, provided as a value, as a 
range, or with a range and selector function. Benchmark will be launched for 
each parameters combination.
```C++
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
Iterations: 1
-------------------------------------------------------------------------------
Phase: std::sort(1000000)
Total time: 66.976 ms
Total items: 1000000
Items throughput: 14930626 / second
-------------------------------------------------------------------------------
Phase: std::sort(10000000)
Total time: 644.141 ms
Total items: 10000000
Items throughput: 15524528 / second
===============================================================================
```

##Example 6: Benchmark class
You can also create a benchmark by inheriting from CppBenchmark::Benchmark class
and implementing Run() method. You can use AddItems() method of a benchmark context 
metrics to register processed items.
```C++
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
Iterations: 1
-------------------------------------------------------------------------------
Phase: std::sort(10000000)
Total time: 648.461 ms
Total items: 10000000
Items throughput: 15421124 / second
===============================================================================
```

##Example 7: Benchmark I/O operations
You can use AddBytes() method of a benchmark context metrics to register processed data.
```C++
#include "cppbenchmark.h"

#include <array>

const int iterations = 100000;
const int chunk_size_from = 32;
const int chunk_size_to = 4096;

// Create settings for the benchmark which will make 100000 iterations for each chunk size 
// scaled from 32 bytes to 4096 bytes (32, 64, 128, 256, 512, 1024, 2048, 4096).
const auto settings = CppBenchmark::Settings()
    .Iterations(iterations)
    .ParamRange(chunk_size_from, chunk_size_to, [](int from, int to, int& result) 
    { 
        int r = result; 
        result *= 2; 
        return r; 
    });

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
Iterations: 100000
-------------------------------------------------------------------------------
Phase: fwrite(32)
Average time: 66 ns / iteration
Minimal time: 66 ns / iteration
Maximal time: 78 ns / iteration
Total time: 6.608 ms
Total iterations: 100000
Total bytes: 3.053 MiB
Iterations throughput: 15131818 / second
Bytes throughput: 461.805 MiB / second
-------------------------------------------------------------------------------
Phase: fwrite(64)
Average time: 93 ns / iteration
Minimal time: 93 ns / iteration
Maximal time: 134 ns / iteration
Total time: 9.380 ms
Total iterations: 100000
Total bytes: 6.106 MiB
Iterations throughput: 10660950 / second
Bytes throughput: 650.709 MiB / second
-------------------------------------------------------------------------------
...
-------------------------------------------------------------------------------
Phase: fwrite(2048)
Average time: 1.846 mcs / iteration
Minimal time: 1.846 mcs / iteration
Maximal time: 2.299 mcs / iteration
Total time: 184.605 ms
Total iterations: 100000
Total bytes: 195.320 MiB
Iterations throughput: 541696 / second
Bytes throughput: 1.034 GiB / second
-------------------------------------------------------------------------------
Phase: fwrite(4096)
Average time: 3.687 mcs / iteration
Minimal time: 3.687 mcs / iteration
Maximal time: 4.617 mcs / iteration
Total time: 368.788 ms
Total iterations: 100000
Total bytes: 390.640 MiB
Iterations throughput: 271157 / second
Bytes throughput: 1.035 GiB / second
===============================================================================
```

#Command line options
When you create and build a benchmark you can run it with the following command line options:
* **-h, --help** - Show help
* **-f, --filter** - Filter benchmarks by the given regexp pattern
* **-l, --list** - List all avaliable benchmarks
* **-o, --output** - Output format (console, csv, json). Default: console
* **-s, --silent** - Launch in silent mode. No progress will be shown!

# Todo
* Doxygen summary
* Github documentation
