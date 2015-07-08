//
// Created by Ivan Shynkarenka on 07.07.2015.
//

#ifndef CPPBENCHMARK_SETTINGS_H
#define CPPBENCHMARK_SETTINGS_H

#include <functional>
#include <tuple>
#include <vector>

namespace CppBenchmark {

class Settings
{
    friend class Benchmark;

public:
    static const Settings Default;

    Settings() = default;
    Settings(const Settings&) = default;
    Settings(Settings&&) = default;
    ~Settings() = default;

    Settings& operator=(const Settings&) = default;
    Settings& operator=(Settings&&) = default;

    Settings& Attempts(int attempts);

    Settings& Iterations(int64_t iterations);
    Settings& Nanoseconds(int64_t nanoseconds);
    Settings& Microseconds(int64_t microseconds);
    Settings& Milliseconds(int64_t milliseconds);
    Settings& Seconds(int64_t seconds);
    Settings& Minutes(int64_t minutes);
    Settings& Hours(int64_t hours);

    Settings& Param(int value);
    Settings& ParamRange(int from, int to);
    Settings& ParamRange(int from, int to, std::function<int (int, int, int)> selector);

    Settings& Pair(int value1, int value2);
    Settings& PairRange(int from1, int to1, int from2, int to2);
    Settings& PairRange(int from1, int to1, std::function<int (int, int, int)> selector1,
                        int from2, int to2, std::function<int (int, int, int)> selector2);

    Settings& Triple(int value1, int value2, int value3);
    Settings& TripleRange(int from1, int to1, int from2, int to2, int from3, int to3);
    Settings& TripleRange(int from1, int to1, std::function<int (int, int, int)> selector1,
                          int from2, int to2, std::function<int (int, int, int)> selector2,
                          int from3, int to3, std::function<int (int, int, int)> selector3);

private:
    int _attempts;
    int64_t _iterations;
    int64_t _nanoseconds;
    std::vector<std::tuple<int, int, int>> _params;
};

} // namespace CppBenchmark

#endif // CPPBENCHMARK_SETTINGS_H
