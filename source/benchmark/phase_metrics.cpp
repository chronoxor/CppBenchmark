/*!
    \file phase_metrics.cpp
    \brief Benchmark phase metrics implementation
    \author Ivan Shynkarenka
    \date 03.07.2015
    \copyright MIT License
*/

#include "benchmark/phase_metrics.h"

#include "benchmark/system.h"

namespace CppBenchmark {

int64_t PhaseMetrics::avg_time() const noexcept
{
    return (_total_iterations > 0) ? (_total_time / _total_iterations) : 0;
}

int64_t PhaseMetrics::min_time() const noexcept
{
    return (_total_iterations > 0) ? (_min_time / _total_iterations) : _min_time;
}

int64_t PhaseMetrics::max_time() const noexcept
{
    return (_total_iterations > 0) ? (_max_time / _total_iterations) : _max_time;
}

int64_t PhaseMetrics::iterations_per_second() const noexcept
{
    if (_total_time <= 0)
        return 0;

    return MulDiv64(_total_iterations, 1000000000, _total_time);
}

int64_t PhaseMetrics::items_per_second() const noexcept
{
    if (_total_time <= 0)
        return 0;

    return MulDiv64(_total_items, 1000000000, _total_time);
}

int64_t PhaseMetrics::bytes_per_second() const noexcept
{
    if (_total_time <= 0)
        return 0;

    return MulDiv64(_total_bytes, 1000000000, _total_time);
}

void PhaseMetrics::StartCollecting() noexcept
{
    _iterstamp = _total_iterations;
    _timestamp = System::Timestamp();
}

void PhaseMetrics::StopCollecting() noexcept
{
    // Get iterations count & duration
    int64_t total_duration = System::Timestamp() - _timestamp;

    // Update time counters
    if (total_duration < _min_time)
        _min_time = total_duration;
    if (total_duration > _max_time)
        _max_time = total_duration;
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
    _custom_uint.insert(metrics._custom_uint.begin(), metrics._custom_uint.end());
    _custom_int64.insert(metrics._custom_int64.begin(), metrics._custom_int64.end());
    _custom_uint64.insert(metrics._custom_uint64.begin(), metrics._custom_uint64.end());
    _custom_flt.insert(metrics._custom_flt.begin(), metrics._custom_flt.end());
    _custom_dbl.insert(metrics._custom_dbl.begin(), metrics._custom_dbl.end());
    _custom_str.insert(metrics._custom_str.begin(), metrics._custom_str.end());

    // Choose best total time with iterations, items and bytes
    if (metrics._total_time < _total_time)
    {
        _total_time = metrics._total_time;
        _total_iterations = metrics._total_iterations;
        _total_items = metrics._total_items;
        _total_bytes = metrics._total_bytes;
        // Overwrite metrics custom tables
        for (auto& it : metrics._custom_int)
            _custom_int[it.first] = it.second;
        for (auto& it : metrics._custom_uint)
            _custom_uint[it.first] = it.second;
        for (auto& it : metrics._custom_int64)
            _custom_int64[it.first] = it.second;
        for (auto& it : metrics._custom_uint64)
            _custom_uint64[it.first] = it.second;
        for (auto& it : metrics._custom_flt)
            _custom_flt[it.first] = it.second;
        for (auto& it : metrics._custom_dbl)
            _custom_dbl[it.first] = it.second;
        for (auto& it : metrics._custom_str)
            _custom_str[it.first] = it.second;
    }
}

uint64_t PhaseMetrics::MulDiv64(uint64_t operant, uint64_t multiplier, uint64_t divider)
{
#if defined(_MSC_VER) && (_MSC_VER == 1900)
#pragma warning(push)
#pragma warning(disable: 4018)
#pragma warning(disable: 4244)
#pragma warning(disable: 4389)
    uint64_t a = operant;
    uint64_t b = multiplier;
    uint64_t c = divider;

    //  Normalize divisor
    unsigned long shift;
    _BitScanReverse64(&shift, c);
    shift = 63 - shift;

    c <<= shift;

    // Multiply
    a = _umul128(a, b, &b);
    if (((b << shift) >> shift) != b)
    {
        // Overflow
        return 0xFFFFFFFFFFFFFFFF;
    }
    b = __shiftleft128(a, b, shift);
    a <<= shift;

    uint32_t div;
    uint32_t q0, q1;
    uint64_t t0, t1;

    // 1st Reduction
    div = (uint32_t)(c >> 32);
    t0 = b / div;
    if (t0 > 0xFFFFFFFF)
        t0 = 0xFFFFFFFF;
    q1 = (uint32_t)t0;
    while (1)
    {
        t0 = _umul128(c, (uint64_t)q1 << 32, &t1);
        if (t1 < b || (t1 == b && t0 <= a))
            break;
        q1--;
    }
    b -= t1;
    if (t0 > a)
        b--;
    a -= t0;

    if (b > 0xFFFFFFFF)
    {
        // Overflow
        return 0xFFFFFFFFFFFFFFFF;
    }

    // 2nd reduction
    t0 = ((b << 32) | (a >> 32)) / div;
    if (t0 > 0xFFFFFFFF)
        t0 = 0xFFFFFFFF;
    q0 = (uint32_t)t0;

    while (1)
    {
        t0 = _umul128(c, q0, &t1);
        if (t1 < b || (t1 == b && t0 <= a))
            break;
        q0--;
    }

    return ((uint64_t)q1 << 32) | q0;
#pragma warning(pop)
#elif defined(__GNUC__)
    __uint128_t a = operant;
    __uint128_t b = multiplier;
    __uint128_t c = divider;

    return (uint64_t)(a * b / c);
#else
    #error MulDiv64 is no supported!
#endif
}

} // namespace CppBenchmark
