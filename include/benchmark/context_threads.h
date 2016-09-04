/*!
    \file context_threads.h
    \brief Benchmark thread running context definition
    \author Ivan Shynkarenka
    \date 21.07.2015
    \copyright MIT License
*/

#ifndef CPPBENCHMARK_CONTEXT_THREADS_H
#define CPPBENCHMARK_CONTEXT_THREADS_H

#include "benchmark/context.h"

namespace CppBenchmark {

//! Benchmark thread running context
/*!
    Provides access to the current threads benchmark context information:
    - Benchmark parameters (x, y, z)
    - Benchmark threads count
    - Benchmark metrics
    - Benchmark cancellation methods
    - Benchmark phase management methods
*/
class ContextThreads : public Context
{
    friend class BenchmarkThreads;

public:
    ContextThreads() = delete;
    ContextThreads(const ContextThreads&) noexcept = default;
    ContextThreads(ContextThreads&&) noexcept = default;
    virtual ~ContextThreads() noexcept = default;

    ContextThreads& operator=(const ContextThreads&) noexcept = default;
    ContextThreads& operator=(ContextThreads&&) noexcept = default;

    //! Benchmark threads count
    int threads() const noexcept { return _threads; }

    //! Get description of the current benchmark running context
    std::string description() const override;

protected:
    //! Benchmark threads count
    int _threads;

    //! Create benchmark running context
    /*!
        \param threads - Benchmark threads count
        \param x - Benchmark first parameter
        \param y - Benchmark second parameter
        \param z - Benchmark third parameter
    */
    ContextThreads(int threads, int x, int y, int z) noexcept
        : Context(x, y, z),
          _threads(threads)
    {}
};

} // namespace CppBenchmark

#endif // CPPBENCHMARK_CONTEXT_THREADS_H
