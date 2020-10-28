/*!
    \file settings.cpp
    \brief Benchmark settings implementation
    \author Ivan Shynkarenka
    \date 07.07.2015
    \copyright MIT License
*/

#include "benchmark/settings.h"

namespace CppBenchmark {

Settings::Settings()
    : _attempts(5),
      _infinite(false),
      _duration(0),
      _operations(0),
      _latency_params(std::make_tuple(0, 0, 0)),
      _latency_auto(false)
{
    Duration(0);
}

Settings::Settings(int64_t operations)
    : Settings()
{
    Operations(operations);
}

Settings& Settings::Attempts(int attempts)
{
    _attempts = (attempts > 0) ? attempts : 5;
    return *this;
}

Settings& Settings::Infinite()
{
    _infinite = true;
    _duration = 0;
    _operations = 0;
    return *this;
}

Settings& Settings::Operations(int64_t operations)
{
    _infinite = false;
    _duration = 0;
    _operations = (operations > 0) ? operations : 1;
    return *this;
}

Settings& Settings::Duration(int64_t duration)
{
    _infinite = false;
    _duration = (duration > 0) ? duration : 5;
    _operations = 0;
    return *this;
}

Settings& Settings::Threads(int threads)
{
    if (threads > 0)
        _threads.emplace_back(threads);
    return *this;
}

Settings& Settings::ThreadsRange(int from, int to)
{
    if ((from > 0) && (to > 0))
    {
        if (from > to)
            std::swap(from, to);

        for (int i = from; i <= to; ++i)
            _threads.emplace_back(i);
    }
    return *this;
}

Settings& Settings::ThreadsRange(int from, int to, const std::function<int (int, int, int&)>& selector)
{
    if ((from > 0) && (to > 0) && selector)
    {
        if (from > to)
            std::swap(from, to);

        // Select the first value
        int current = from;
        int result = selector(from, to, current);
        while ((result >= from) && (result <= to))
        {
            _threads.emplace_back(result);

            // Select the next value
            result = selector(from, to, current);
        }
    }
    return *this;
}

Settings& Settings::PC(int producers, int consumers)
{
    if ((producers > 0) && (consumers > 0))
        _pc.emplace_back(producers, consumers);
    return *this;
}

Settings& Settings::PCRange(int producers_from, int producers_to, int consumers_from, int consumers_to)
{
    if ((producers_from > 0) && (producers_to > 0) && (consumers_from > 0) && (consumers_to > 0))
    {
        if (producers_from > producers_to)
            std::swap(producers_from, producers_to);

        if (consumers_from > consumers_to)
            std::swap(consumers_from, consumers_to);

        for (int i = producers_from; i <= producers_to; ++i)
            for (int j = consumers_from; j <= consumers_to; ++j)
                _pc.emplace_back(i, j);
    }
    return *this;
}

Settings& Settings::PCRange(int producers_from, int producers_to, const std::function<int (int, int, int&)>& producers_selector,
                            int consumers_from, int consumers_to, const std::function<int (int, int, int&)>& consumers_selector)
{
    if ((producers_from > 0) && (producers_to > 0) && (consumers_from > 0) && (consumers_to > 0) && producers_selector && consumers_selector)
    {
        if (producers_from > producers_to)
            std::swap(producers_from, producers_to);

        if (consumers_from > consumers_to)
            std::swap(consumers_from, consumers_to);

        // Select the first value
        int current1 = producers_from;
        int result1 = producers_selector(producers_from, producers_to, current1);
        while ((result1 >= producers_from) && (result1 <= producers_to))
        {
            // Select the second value
            int current2 = consumers_from;
            int result2 = consumers_selector(consumers_from, consumers_to, current2);
            while ((result2 >= consumers_from) && (result2 <= consumers_to))
            {
                _pc.emplace_back(result1, result2);

                // Select the next value
                result2 = consumers_selector(consumers_from, consumers_to, current2);
            }

            // Select the next value
            result1 = producers_selector(producers_from, producers_to, current1);
        }
    }
    return *this;
}

Settings& Settings::Param(int value)
{
    if (value >= 0)
        _params.emplace_back(value, -1, -1);
    return *this;
}

Settings& Settings::ParamRange(int from, int to)
{
    if ((from >= 0) && (to >= 0))
    {
        if (from > to)
            std::swap(from, to);

        for (int i = from; i <= to; ++i)
            _params.emplace_back(i, -1, -1);
    }
    return *this;
}

Settings& Settings::ParamRange(int from, int to, const std::function<int (int, int, int&)>& selector)
{
    if ((from >= 0) && (to >= 0) && selector)
    {
        if (from > to)
            std::swap(from, to);

        // Select the first value
        int current = from;
        int result = selector(from, to, current);
        while ((result >= from) && (result <= to))
        {
            _params.emplace_back(result, -1, -1);

            // Select the next value
            result = selector(from, to, current);
        }
    }
    return *this;
}

Settings& Settings::Pair(int value1, int value2)
{
    if ((value1 >= 0) && (value2 >= 0))
        _params.emplace_back(value1, value2, -1);
    return *this;
}

Settings& Settings::PairRange(int from1, int to1, int from2, int to2)
{
    if ((from1 >= 0) && (to1 >= 0) && (from2 >= 0) && (to2 >= 0))
    {
        if (from1 > to1)
            std::swap(from1, to1);

        if (from2 > to2)
            std::swap(from2, to2);

        for (int i = from1; i <= to1; ++i)
            for (int j = from2; j <= to2; ++j)
                _params.emplace_back(i, j, -1);
    }
    return *this;
}

Settings& Settings::PairRange(int from1, int to1, const std::function<int (int, int, int&)>& selector1,
                              int from2, int to2, const std::function<int (int, int, int&)>& selector2)
{
    if ((from1 >= 0) && (to1 >= 0) && (from2 >= 0) && (to2 >= 0) && selector1 && selector2)
    {
        if (from1 > to1)
            std::swap(from1, to1);

        if (from2 > to2)
            std::swap(from2, to2);

        // Select the first value
        int current1 = from1;
        int result1 = selector1(from1, to1, current1);
        while ((result1 >= from1) && (result1 <= to1))
        {
            // Select the second value
            int current2 = from2;
            int result2 = selector2(from2, to2, current2);
            while ((result2 >= from2) && (result2 <= to2))
            {
                _params.emplace_back(result1, result2, -1);

                // Select the next value
                result2 = selector2(from2, to2, current2);
            }

            // Select the next value
            result1 = selector1(from1, to1, current1);
        }
    }
    return *this;
}

Settings& Settings::Triple(int value1, int value2, int value3)
{
    if ((value1 >= 0) && (value2 >= 0) && (value3 >= 0))
        _params.emplace_back(value1, value2, value3);
    return *this;
}

Settings& Settings::TripleRange(int from1, int to1, int from2, int to2, int from3, int to3)
{
    if ((from1 >= 0) && (to1 >= 0) && (from2 >= 0) && (to2 >= 0) && (from3 >= 0) && (to3 >= 0))
    {
        if (from1 > to1)
            std::swap(from1, to1);

        if (from2 > to2)
            std::swap(from2, to2);

        if (from3 > to3)
            std::swap(from3, to3);

        for (int i = from1; i <= to1; ++i)
            for (int j = from2; j <= to2; ++j)
                for (int k = from3; k <= to3; ++k)
                    _params.emplace_back(i, j, k);
    }
    return *this;
}

Settings& Settings::TripleRange(int from1, int to1, const std::function<int (int, int, int&)>& selector1,
                                int from2, int to2, const std::function<int (int, int, int&)>& selector2,
                                int from3, int to3, const std::function<int (int, int, int&)>& selector3)
{
    if ((from1 >= 0) && (to1 >= 0) && (from2 >= 0) && (to2 >= 0) && (from3 >= 0) && (to3 >= 0) && selector1 && selector2 && selector3)
    {
        if (from1 > to1)
            std::swap(from1, to1);

        if (from2 > to2)
            std::swap(from2, to2);

        if (from3 > to3)
            std::swap(from3, to3);

        // Select the first value
        int current1 = from1;
        int result1 = selector1(from1, to1, current1);
        while ((result1 >= from1) && (result1 <= to1))
        {
            // Select the second value
            int current2 = from2;
            int result2 = selector2(from2, to2, current2);
            while ((result2 >= from2) && (result2 <= to2))
            {
                // Select the third value
                int current3 = from3;
                int result3 = selector3(from3, to3, current3);
                while ((result3 >= from3) && (result3 <= to3))
                {
                    _params.emplace_back(result1, result2, result3);

                    // Select the next value
                    result3 = selector3(from3, to3, current3);
                }

                // Select the next value
                result2 = selector2(from2, to2, current2);
            }

            // Select the next value
            result1 = selector1(from1, to1, current1);
        }
    }
    return *this;
}

Settings& Settings::Latency(int64_t lowest, int64_t highest, int significant, bool automatic)
{
    _latency_params = std::make_tuple(lowest, highest, significant);
    _latency_auto = automatic;
    return *this;
}

} // namespace CppBenchmark
