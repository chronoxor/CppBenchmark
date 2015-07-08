//
// Created by Ivan Shynkarenka on 07.07.2015.
//

#ifndef CPPBENCHMARK_CONTEXT_H
#define CPPBENCHMARK_CONTEXT_H

namespace CppBenchmark {

class Context
{
    friend class Benchmark;

public:
    Context() = delete;
    Context(const Context&) = default;
    Context(Context&&) = default;
    ~Context() = default;

    Context& operator=(const Context&) = default;
    Context& operator=(Context&&) = default;

    int x() const { return _x; }
    int y() const { return _y; }
    int z() const { return _z; }

private:
    int _x;
    int _y;
    int _z;

    Context(int x, int y, int z) : _x(x), _y(y), _z(z) {}
};

} // namespace CppBenchmark

#endif // CPPBENCHMARK_CONTEXT_H
