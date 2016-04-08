/*!
    \file context_thread.h
    \brief Benchmark thread running context definition
    \author Ivan Shynkarenka
    \date 21.07.2015
    \copyright MIT License
*/

#ifndef CPPBENCHMARK_CONTEXT_THREAD_H
#define CPPBENCHMARK_CONTEXT_THREAD_H

#include "context.h"

namespace CppBenchmark {

//! Benchmark thread running context
/*!
    Provides access to the current thread benchmark context information:
    - Benchmark parameters (x, y, z)
    - Benchmark threads count
    - Benchmark metrics
    - Benchmark cancellation methods
    - Benchmark phase management methods
*/
class ContextThread : public Context
{
    friend class BenchmarkThreads;

public:
    ContextThread() = delete;
    ContextThread(const ContextThread&) noexcept = default;
    ContextThread(ContextThread&&) noexcept = default;
    ~ContextThread() noexcept = default;

    ContextThread& operator=(const ContextThread&) noexcept = default;
    ContextThread& operator=(ContextThread&&) noexcept = default;

    //! Benchmark threads count
    int threads() const noexcept { return _threads; }

    //! Get name of the current benchmark running context
    std::string to_string() const override;

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
    ContextThread(int threads, int x, int y, int z) noexcept : Context(x, y, z), _threads(threads) {}
};

} // namespace CppBenchmark

#endif // CPPBENCHMARK_CONTEXT_THREAD_H
