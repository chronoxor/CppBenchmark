/*!
    \file context_thread.cpp
    \brief Benchmark thread running context implementation
    \author Ivan Shynkarenka
    \date 21.07.2015
    \copyright MIT License
*/

#include "benchmark/context_thread.h"

namespace CppBenchmark {

std::string ContextThread::to_string() const
{
    if ((_x < 0) && (_y < 0) && (_z < 0))
        return "(threads:" + std::to_string(_threads) + ")";
    else if ((_y < 0) && (_z < 0))
        return "(threads:" + std::to_string(_threads) + "," + std::to_string(_x) + ")";
    else if ((_z < 0))
        return "(threads:" + std::to_string(_threads) + "," + std::to_string(_x) + "," + std::to_string(_y) + ")";
    else
        return "(threads:" + std::to_string(_threads) + "," + std::to_string(_x) + "," + std::to_string(_y) + "," + std::to_string(_z) + ")";
}

} // namespace CppBenchmark
