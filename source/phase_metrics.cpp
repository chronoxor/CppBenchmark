//
// Created by Ivan Shynkarenka on 03.07.2015.
//

#include "phase_metrics.h"

namespace CppBenchmark {

void PhaseMetrics::StartIteration() noexcept
{
    // Do nothing if already started!
    if (_is_started)
        return;

    // Start high resolution timer
    _is_started = true;
    _start_time = std::chrono::high_resolution_clock::now();
    _start_iteration = _total_iterations;
}

void PhaseMetrics::StopIteration() noexcept
{
    // Do nothing if not already started!
    if (!_is_started)
        return;

    // Stop high resolution timer
    _stop_time = std::chrono::high_resolution_clock::now();
    _is_started = false;

    // Get phase duration
    int64_t duration = std::chrono::duration_cast<std::chrono::nanoseconds>(_stop_time - _start_time).count();

    // Get phase iterations count
    int64_t iterations = _total_iterations - _start_iteration;

    // Get phase iteration delta
    int64_t delta = duration / iterations;

    // Update time counters
    if (delta < _min_time)
        _min_time = delta;
    if (delta > _max_time)
        _max_time = delta;
    _total_time += duration;
}

} // namespace CppBenchmark