# CppBenchmark
C++ Benchmark Library

#Contents
  * [Requirements](#requirements)
  * [How to build?](#how-to-build)
    * [Windows (Visaul Studio 2015)](#windows-visaul-studio-2015)
    * [Windows (MinGW with MSYS)](#windows-mingw-with-msys)
    * [Linux](#linux)
  * [How to use?](#how-to-use)
  * [Example 1: Micro benchmark of a function call](#example-1-micro-benchmark-of-a-function-call)
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

#How to use?
##Example 1: Micro benchmark of a function call
```C++
#include "cppbenchmark.h"

#include <math.h>

// Micro benchmark a sin() call for 100000000 times. 
// Make 5 attemtps (by default) and choose one with the best time result.
BENCHMARK("sin", 100000000)
{
    sin((rand() / (RAND_MAX + 1.0)));
}

BENCHMARK_MAIN()
```

# Todo
* Doxygen summary
* Github documentation
