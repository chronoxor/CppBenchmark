/*!
    \file phase_metrics.cpp
    \brief Benchmark phase metrics implementation
    \author Ivan Shynkarenka
    \date 03.07.2015
    \copyright MIT License
*/

#include "phase_metrics.h"
#include "system.h"

namespace CppBenchmark {

int64_t PhaseMetrics::avg_time() const noexcept
{
    return (_total_iterations > 0) ? (_total_time / _total_iterations) : 0;
}

int64_t PhaseMetrics::min_time() const noexcept
{
    return _min_time;
}

int64_t PhaseMetrics::max_time() const noexcept
{
    return _max_time;
}

int64_t PhaseMetrics::iterations_per_second() const noexcept
{
    if (_total_time <= 0)
        return 0;

    return (_total_iterations * 1000000000) / _total_time;
}

int64_t PhaseMetrics::items_per_second() const noexcept
{
    if (_total_time <= 0)
        return 0;

    return (_total_items * 1000000000) / _total_time;
}

int64_t PhaseMetrics::bytes_per_second() const noexcept
{
    if (_total_time <= 0)
        return 0;

    return (_total_bytes * 1000000000) / _total_time;
}

void PhaseMetrics::StartCollecting() noexcept
{
    _iterstamp = _total_iterations;
    _timestamp = System::Timestamp();
}

void PhaseMetrics::StopCollecting() noexcept
{
    // Get iterations count & duration
    int64_t iterations = _total_iterations - _iterstamp;
    int64_t total_duration = System::Timestamp() - _timestamp;
    int64_t average_duration = total_duration / ((iterations > 0) ? iterations : 1);

    // Update time counters
    if (average_duration < _min_time)
        _min_time = average_duration;
    if (average_duration > _max_time)
        _max_time = average_duration;
    _total_time += total_duration;
}

void PhaseMetrics::MergeMetrics(const PhaseMetrics& metrics)
{
    // Choose best min time
    if (metrics._min_time < _min_time)
        _min_time = metrics._min_time;

    // Choose best max time
    if (metrics._max_time > _max_time)
        _max_time = metrics._max_time;

    // Merge custom hash tables
    _custom_int.insert(metrics._custom_int.begin(), metrics._custom_int.end());
    _custom_str.insert(metrics._custom_str.begin(), metrics._custom_str.end());

    // Choose best total time with iterations, items and bytes
    if (metrics._total_time < _total_time)
    {
        _total_time = metrics._total_time;
        _total_iterations = metrics._total_iterations;
        _total_items = metrics._total_items;
        _total_bytes = metrics._total_bytes;
        // Overwrite metrics custom hash tables
        for (auto& it : metrics._custom_int)
            _custom_int[it.first] = it.second;
        for (auto& it : metrics._custom_str)
            _custom_str[it.first] = it.second;
    }
}

} // namespace CppBenchmark
