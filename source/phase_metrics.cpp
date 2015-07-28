//
// Created by Ivan Shynkarenka on 03.07.2015.
//

#include "phase_metrics.h"

namespace CppBenchmark {

int64_t PhaseMetrics::avg_time() const noexcept
{
    return (_total_iterations > 0) ? (_total_time / _total_iterations) : 0;
}

int64_t PhaseMetrics::min_time() const noexcept
{
    return (_total_iterations > 0) ? (_min_time / _total_iterations) : 0;
}

int64_t PhaseMetrics::max_time() const noexcept
{
    return (_total_iterations > 0) ? (_max_time / _total_iterations) : 0;
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

void PhaseMetrics::StartCollecting() noexcept
{
    _timestamp = std::chrono::high_resolution_clock::now(); 
}

void PhaseMetrics::StopCollecting() noexcept
{
    // Get iteration duration
    int64_t duration = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - _timestamp).count();

    // Update time counters
    if (duration < _min_time)
        _min_time = duration;
    if (duration > _max_time)
        _max_time = duration;
    _total_time += duration;
}

} // namespace CppBenchmark
