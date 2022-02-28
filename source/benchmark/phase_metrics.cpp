/*!
    \file phase_metrics.cpp
    \brief Benchmark phase metrics implementation
    \author Ivan Shynkarenka
    \date 03.07.2015
    \copyright MIT License
*/

#include "benchmark/phase_metrics.h"

#include "benchmark/system.h"

#include <algorithm>
#include <cstdlib>

#if defined(_MSC_VER)
#include <intrin.h>
#endif

#include <hdr/hdr_histogram.h>

namespace CppBenchmark {

PhaseMetrics::PhaseMetrics() : _histogram(nullptr)
{
    ResetMetrics();
}

PhaseMetrics::~PhaseMetrics()
{
    FreeLatencyHistogram();
}

bool PhaseMetrics::latency() const noexcept
{
    return (_histogram != nullptr);
}

int64_t PhaseMetrics::min_latency() const noexcept
{
    return latency() ? hdr_min((const hdr_histogram*)_histogram) : 0;
}

int64_t PhaseMetrics::max_latency() const noexcept
{
    return latency() ? hdr_max((const hdr_histogram*)_histogram) : 0;
}

double PhaseMetrics::mean_latency() const noexcept
{
    return latency() ? hdr_mean((const hdr_histogram*)_histogram) : 0;
}

double PhaseMetrics::stdv_latency() const noexcept
{
    return latency() ? hdr_stddev((const hdr_histogram*)_histogram) : 0;
}

int64_t PhaseMetrics::avg_time() const noexcept
{
    return (_total_operations > 0) ? (_total_time / _total_operations) : 0;
}

int64_t PhaseMetrics::min_time() const noexcept
{
    return _min_time;
}

int64_t PhaseMetrics::max_time() const noexcept
{
    return _max_time;
}

int64_t PhaseMetrics::operations_per_second() const noexcept
{
    if (_total_time <= 0)
        return 0;

    return System::MulDiv64(_total_operations, 1000000000, _total_time);
}

int64_t PhaseMetrics::items_per_second() const noexcept
{
    if (_total_time <= 0)
        return 0;

    return System::MulDiv64(_total_items, 1000000000, _total_time);
}

int64_t PhaseMetrics::bytes_per_second() const noexcept
{
    if (_total_time <= 0)
        return 0;

    return System::MulDiv64(_total_bytes, 1000000000, _total_time);
}

void PhaseMetrics::InitLatencyHistogram(const std::tuple<int64_t, int64_t, int>& latency) noexcept
{
    int64_t lowest = std::get<0>(latency);
    int64_t highest = std::get<1>(latency);
    int significant = std::get<2>(latency);

    FreeLatencyHistogram();
    int result = hdr_init(lowest, highest, significant, ((hdr_histogram**)&_histogram));
    if (result != 0)
        _histogram = nullptr;
}

void PhaseMetrics::PrintLatencyHistogram(FILE* file, int32_t resolution) const noexcept
{
    if ((_histogram != nullptr) && (file != nullptr))
    {
        hdr_percentiles_print((hdr_histogram*)_histogram, file, resolution, 1.0, CLASSIC);
    }
}

void PhaseMetrics::FreeLatencyHistogram() noexcept
{
    if (_histogram != nullptr)
    {
        hdr_close((hdr_histogram*)_histogram);
        _histogram = nullptr;
    }
}

void PhaseMetrics::AddLatency(int64_t latency) noexcept
{
    if (_histogram != nullptr)
        hdr_record_values((hdr_histogram*)_histogram, latency, 1);
}

void PhaseMetrics::StartCollecting() noexcept
{
    _iterstamp = _total_operations;
    _timestamp = System::Timestamp();
}

void PhaseMetrics::StopCollecting() noexcept
{
    // Get iterations count & duration of the phase
    int64_t iterations = _total_operations - _iterstamp;
    int64_t duration = System::Timestamp() - _timestamp;

    // Get min & max time of the phase
    int64_t min_time = (iterations > 0) ? (duration / iterations) : std::numeric_limits<int64_t>::max();
    int64_t max_time = (iterations > 0) ? (duration / iterations) : std::numeric_limits<int64_t>::min();

    // Update time counters
    if (min_time < _min_time)
        _min_time = min_time;
    if (max_time > _max_time)
        _max_time = max_time;
    _total_time += duration;
}

void PhaseMetrics::MergeMetrics(PhaseMetrics& metrics)
{
    // Choose best min time
    if (metrics._min_time < _min_time)
        _min_time = metrics._min_time;

    // Choose best max time
    if (metrics._max_time > _max_time)
        _max_time = metrics._max_time;

    // Merge custom hash tables
    _custom_int.insert(metrics._custom_int.begin(), metrics._custom_int.end());
    _custom_uint.insert(metrics._custom_uint.begin(), metrics._custom_uint.end());
    _custom_int64.insert(metrics._custom_int64.begin(), metrics._custom_int64.end());
    _custom_uint64.insert(metrics._custom_uint64.begin(), metrics._custom_uint64.end());
    _custom_flt.insert(metrics._custom_flt.begin(), metrics._custom_flt.end());
    _custom_dbl.insert(metrics._custom_dbl.begin(), metrics._custom_dbl.end());
    _custom_str.insert(metrics._custom_str.begin(), metrics._custom_str.end());

    // Choose best total time with operations, items and bytes
    if (metrics._total_time < _total_time)
    {
        std::swap(_histogram, metrics._histogram);
        _total_time = metrics._total_time;
        _total_operations = metrics._total_operations;
        _total_items = metrics._total_items;
        _total_bytes = metrics._total_bytes;

        // Overwrite metrics custom tables
        for (const auto& it : metrics._custom_int)
            _custom_int[it.first] = it.second;
        for (const auto& it : metrics._custom_uint)
            _custom_uint[it.first] = it.second;
        for (const auto& it : metrics._custom_int64)
            _custom_int64[it.first] = it.second;
        for (const auto& it : metrics._custom_uint64)
            _custom_uint64[it.first] = it.second;
        for (const auto& it : metrics._custom_flt)
            _custom_flt[it.first] = it.second;
        for (const auto& it : metrics._custom_dbl)
            _custom_dbl[it.first] = it.second;
        for (const auto& it : metrics._custom_str)
            _custom_str[it.first] = it.second;

        // Overwrite metrics threads value
        _threads = metrics._threads;
    }
}

void PhaseMetrics::ResetMetrics() noexcept
{
    FreeLatencyHistogram();
    _min_time = std::numeric_limits<int64_t>::max();
    _max_time = std::numeric_limits<int64_t>::min();
    _total_time = 0;
    _total_operations = 0;
    _total_items = 0;
    _total_bytes = 0;
    _iterstamp = 0;
    _timestamp = 0;
    _threads = 1;
}

} // namespace CppBenchmark
