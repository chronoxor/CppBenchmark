//
// Created by Ivan Shynkarenka on 03.07.2015.
//

#include "phase_metrics.h"

namespace CppBenchmark {

void PhaseMetrics::StartIteration()
{
    // Do nothing if already started!
    if (_is_started)
        return;

    // Update iterations counter
    _total_iterations += 1;

    // Start high resolution timer
    _is_started = true;
    _start_time = std::chrono::high_resolution_clock::now();
}

void PhaseMetrics::StopIteration()
{
    // Do nothing if not already started!
    if (!_is_started)
        return;

    // Stop high resolution timer
    _stop_time = std::chrono::high_resolution_clock::now();
    _is_started = false;

    // Get phase duration
    int64_t duration = std::chrono::duration_cast<std::chrono::nanoseconds>(_stop_time - _start_time).count();

    // Update time counters
    if (duration < _min_time)
        _min_time = duration;
    if (duration > _max_time)
        _max_time = duration;
    _total_time += duration;
}

} // namespace CppBenchmark