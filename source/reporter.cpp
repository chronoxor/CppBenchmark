//
// Created by Ivan Shynkarenka on 08.07.2015.
//

#include "reporter.h"

namespace CppBenchmark {

std::string to_string(MetricsReportingType value)
{
    switch (value)
    {
        case eReportBest:
            return "best";
        case eReportWorst:
            return "worst";
        case eReportDiff:
            return "diff";
        default:
            return "<unknown>";
    }
}

} // namespace CppBenchmark
