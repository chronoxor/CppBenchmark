//
// Created by Ivan Shynkarenka on 07.07.2015.
//

#ifndef CPPBENCHMARK_CONTEXT_H
#define CPPBENCHMARK_CONTEXT_H

#include <string>

namespace CppBenchmark {

class Context
{
    friend class Launcher;

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

    friend std::string to_string(const Context& instance);

private:
    int _x;
    int _y;
    int _z;

    Context(int x, int y, int z) : _x(x), _y(y), _z(z) {}
};

} // namespace CppBenchmark

#endif // CPPBENCHMARK_CONTEXT_H
