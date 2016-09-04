/*!
    \file context_pc.cpp
    \brief Benchmark producer/consumer running context implementation
    \author Ivan Shynkarenka
    \date 24.07.2015
    \copyright MIT License
*/

#include "benchmark/context_pc.h"

namespace CppBenchmark {

std::string ContextPC::description() const
{
    if ((_x < 0) && (_y < 0) && (_z < 0))
        return "(producers:" + std::to_string(_producers) + ",consumers:" + std::to_string(_consumers) + ")";
    else if ((_y < 0) && (_z < 0))
        return "(producers:" + std::to_string(_producers) + ",consumers:" + std::to_string(_consumers) + "," + std::to_string(_x) + ")";
    else if ((_z < 0))
        return "(producers:" + std::to_string(_producers) + ",consumers:" + std::to_string(_consumers) + "," + std::to_string(_x) + "," + std::to_string(_y) + ")";
    else
        return "(producers:" + std::to_string(_producers) + ",consumers:" + std::to_string(_consumers) + "," + std::to_string(_x) + "," + std::to_string(_y) + "," + std::to_string(_z) + ")";
}

} // namespace CppBenchmark
