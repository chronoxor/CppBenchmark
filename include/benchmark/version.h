/*!
    \file version.h
    \brief Version definition
    \author Ivan Shynkarenka
    \date 13.07.2015
    \copyright MIT License
*/

#ifndef CPPBENCHMARK_VERSION_H
#define CPPBENCHMARK_VERSION_H

/*! \mainpage C++ Benchmark Library

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

/*!
    \namespace CppBenchmark
    \brief C++ Benchmark project definitions
*/
namespace CppBenchmark {

//! Project version
const char version[] = "1.0.4.0";

} // namespace CppBenchmark

#endif // CPPBENCHMARK_VERSION_H
