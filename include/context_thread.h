//
// Created by Ivan Shynkarenka on 21.07.2015.
//

#ifndef CPPBENCHMARK_CONTEXT_THREAD_H
#define CPPBENCHMARK_CONTEXT_THREAD_H

#include "context.h"

namespace CppBenchmark {

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

    int threads() const noexcept { return _threads; }

    std::string to_string() const override;

private:
    int _threads;

    ContextThread(int threads, int x, int y, int z) noexcept
            : Context(x, y, z),
              _threads(threads)
    {}
};

} // namespace CppBenchmark

#endif // CPPBENCHMARK_CONTEXT_THREAD_H
