//
// Created by Ivan Shynkarenka on 07.07.2015.
//

#include "settings.h"

namespace CppBenchmark {

const Settings Settings::Default = Settings().Attempts(10);

Settings& Settings::Attempts(int attempts)
{
    _attempts = (attempts > 0) ? attempts : 10;
    return *this;
}

Settings& Settings::Iterations(int64_t iterations)
{
    _iterations = (iterations > 0) ? iterations : 0;
    return *this;
}

Settings& Settings::Nanoseconds(int64_t nanoseconds)
{
    _nanoseconds = (nanoseconds > 0) ? nanoseconds : 0;
    return *this;
}

Settings& Settings::Microseconds(int64_t microseconds)
{
    _nanoseconds = (microseconds > 0) ? (microseconds * 1000) : 0;
    return *this;
}

Settings& Settings::Milliseconds(int64_t milliseconds)
{
    _nanoseconds = (milliseconds > 0) ? (milliseconds * 1000 * 1000) : 0;
    return *this;
}

Settings& Settings::Seconds(int64_t seconds)
{
    _nanoseconds = (seconds > 0) ? (seconds * 1000 * 1000 * 1000) : 0;
    return *this;
}

Settings& Settings::Minutes(int64_t minutes)
{
    _nanoseconds = (minutes > 0) ? (minutes * 1000 * 1000 * 1000 * 60) : 0;
    return *this;
}

Settings& Settings::Hours(int64_t hours)
{
    _nanoseconds = (hours > 0) ? (hours * 1000 * 1000 * 1000 * 60 * 60) : 0;
    return *this;
}

Settings& Settings::Param(int value)
{
    if (value >= 0)
        _params.emplace_back(std::tuple<int, int, int>(value, -1, -1));
    return *this;
}

Settings& Settings::ParamRange(int from, int to)
{
    if ((from >= 0) && (to >= 0)) {
        if (from > to) {
            from = to;
            to = from;
        }
        for (int i = from; i < to; ++i)
            _params.emplace_back(std::tuple<int, int, int>(i, -1, -1));
    }
    return *this;
}

Settings& Settings::ParamRange(int from, int to, std::function<int (int, int, int)> selector)
{
    if ((from >= 0) && (to >= 0)) {
        if (from > to) {
            from = to;
            to = from;
        }
        // Select the first value
        int result = selector(from, to, -1);
        while ((result >= from) && (result <= to)) {
            _params.emplace_back(std::tuple<int, int, int>(result, -1, -1));
            // Select the next value
            result = selector(from, to, result);
        }
    }
    return *this;
}

Settings& Settings::Pair(int value1, int value2)
{
    if ((value1 >= 0) && (value2 >= 0))
        _params.emplace_back(std::tuple<int, int, int>(value1, value2, -1));
    return *this;
}

Settings& Settings::PairRange(int from1, int to1, int from2, int to2)
{
    if ((from1 >= 0) && (to1 >= 0) && (from2 >= 0) && (to2 >= 0)) {
        if (from1 > to1) {
            from1 = to1;
            to1 = from1;
        }
        if (from2 > to2) {
            from2 = to2;
            to2 = from2;
        }
        for (int i = from1; i < to1; ++i)
            for (int j = from2; i < to2; ++j)
                _params.emplace_back(std::tuple<int, int, int>(i, j, -1));
    }
    return *this;
}

Settings& Settings::PairRange(int from1, int to1, std::function<int (int, int, int)> selector1,
                              int from2, int to2, std::function<int (int, int, int)> selector2)
{
    if ((from1 >= 0) && (to1 >= 0) && (from2 >= 0) && (to2 >= 0)) {
        if (from1 > to1) {
            from1 = to1;
            to1 = from1;
        }
        if (from2 > to2) {
            from2 = to2;
            to2 = from2;
        }
        // Select the first value
        int result1 = selector1(from1, to1, -1);
        while ((result1 >= from1) && (result1 <= to1)) {
            // Select the second value
            int result2 = selector2(from2, to2, -1);
            while ((result2 >= from2) && (result2 <= to2)) {
                _params.emplace_back(std::tuple<int, int, int>(result1, result2, -1));
                // Select the next value
                result2 = selector2(from2, to2, result2);
            }
            // Select the next value
            result1 = selector1(from1, to1, result1);
        }
    }
    return *this;
}

Settings& Settings::Triple(int value1, int value2, int value3)
{
    if ((value1 >= 0) && (value2 >= 0) && (value3 >= 0))
        _params.emplace_back(std::tuple<int, int, int>(value1, value2, value3));
    return *this;
}

Settings& Settings::TripleRange(int from1, int to1, int from2, int to2, int from3, int to3)
{
    if ((from1 >= 0) && (to1 >= 0) && (from2 >= 0) && (to2 >= 0) && (from3 >= 0) && (to3 >= 0)) {
        if (from1 > to1) {
            from1 = to1;
            to1 = from1;
        }
        if (from2 > to2) {
            from2 = to2;
            to2 = from2;
        }
        if (from3 > to3) {
            from3 = to3;
            to3 = from3;
        }
        for (int i = from1; i < to1; ++i)
            for (int j = from2; i < to2; ++j)
                for (int k = from3; i < to3; ++k)
                    _params.emplace_back(std::tuple<int, int, int>(i, j, k));
    }
    return *this;
}

Settings& Settings::TripleRange(int from1, int to1, std::function<int (int, int, int)> selector1,
                                int from2, int to2, std::function<int (int, int, int)> selector2,
                                int from3, int to3, std::function<int (int, int, int)> selector3)
{
    if ((from1 >= 0) && (to1 >= 0) && (from2 >= 0) && (to2 >= 0) && (from3 >= 0) && (to3 >= 0)) {
        if (from1 > to1) {
            from1 = to1;
            to1 = from1;
        }
        if (from2 > to2) {
            from2 = to2;
            to2 = from2;
        }
        if (from3 > to3) {
            from3 = to3;
            to3 = from3;
        }
        // Select the first value
        int result1 = selector1(from1, to1, -1);
        while ((result1 >= from1) && (result1 <= to1)) {
            // Select the second value
            int result2 = selector2(from2, to2, -1);
            while ((result2 >= from2) && (result2 <= to2)) {
                // Select the third value
                int result3 = selector3(from3, to3, -1);
                while ((result3 >= from3) && (result3 <= to3)) {
                    _params.emplace_back(std::tuple<int, int, int>(result1, result2, result3));
                    // Select the next value
                    result3 = selector3(from3, to3, result3);
                }
                // Select the next value
                result2 = selector2(from2, to2, result2);
            }
            // Select the next value
            result1 = selector1(from1, to1, result1);
        }
    }
    return *this;
}

} // namespace CppBenchmark
