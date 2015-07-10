//
// Created by Ivan Shynkarenka on 07.07.2015.
//

#include "context.h"

namespace CppBenchmark {

std::string to_string(const Context& instance)
{
    if ((instance.x() < 0) && (instance.y() < 0) && (instance.z() < 0))
        return "()";
    else if ((instance.y() < 0) && (instance.z() < 0))
        return '(' + std::to_string(instance.x()) + ')';
    else if ((instance.z() < 0))
        return '(' + std::to_string(instance.x()) + ", " + std::to_string(instance.y()) + ')';
    else
        return '(' + std::to_string(instance.x()) + ", " + std::to_string(instance.y()) + ", " + std::to_string(instance.z()) + ')';
}

} // namespace CppBenchmark
