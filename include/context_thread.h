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
    ContextThread(const ContextThread&) = default;
    ContextThread(ContextThread&&) = default;
    ~ContextThread() = default;

    ContextThread& operator=(const ContextThread&) = default;
    ContextThread& operator=(ContextThread&&) = default;

    int threads() const { return _threads; }

    std::string to_string() const override;

private:
    int _threads;

    ContextThread(int threads, int x, int y, int z) : Context(x, y, z), _threads(threads) {}
};

} // namespace CppBenchmark

#endif // CPPBENCHMARK_CONTEXT_THREAD_H
