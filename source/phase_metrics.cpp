//
// Created by Ivan Shynkarenka on 03.07.2015.
//

#include "phase_metrics.h"

namespace CppBenchmark {

int64_t PhaseMetrics::time_per_iteration() const noexcept
{
    return (_total_iterations > 0) ? (_total_time / _total_iterations) : 0;
}

int64_t PhaseMetrics::iterations_per_second() const noexcept
{
    if (_total_time <= 0)
        return 0;

    if (_total_iterations < 1000000000)
        return (_total_iterations * 1000000000) / _total_time;
    else
        return (_total_iterations / _total_time) * 1000000000;
}

int64_t PhaseMetrics::items_per_second() const noexcept
{
    if (_total_time <= 0)
        return 0;

    if (_total_items < 1000000000)
        return (_total_items * 1000000000) / _total_time;
    else
        return (_total_items / _total_time) * 1000000000;
}

int64_t PhaseMetrics::bytes_per_second() const noexcept
{
    if (_total_time <= 0)
        return 0;

    if (_total_bytes < 1000000000)
        return (_total_bytes * 1000000000) / _total_time;
    else
        return (_total_bytes / _total_time) * 1000000000;
}

} // namespace CppBenchmark
