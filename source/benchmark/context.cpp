/*!
    \file context.cpp
    \brief Benchmark running context implementation
    \author Ivan Shynkarenka
    \date 07.07.2015
    \copyright MIT License
*/

#include "benchmark/context.h"

namespace CppBenchmark {

std::string Context::description() const
{
    if ((_x < 0) && (_y < 0) && (_z < 0))
        return "";
    else if ((_y < 0) && (_z < 0))
        return "(" + std::to_string(_x) + ")";
    else if ((_z < 0))
        return "(" + std::to_string(_x) + "," + std::to_string(_y) + ")";
    else
        return "(" + std::to_string(_x) + "," + std::to_string(_y) + "," + std::to_string(_z) + ")";
}

} // namespace CppBenchmark
