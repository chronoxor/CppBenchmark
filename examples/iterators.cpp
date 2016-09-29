//
// Created by Ivan Shynkarenka on 03.08.2015.
//

#include "benchmark/cppbenchmark.h"

#include <list>
#include <vector>

const uint64_t iterations = 1000;

template <typename T>
class ContainerPreset
{
protected:
    T container;

    ContainerPreset()
    {
        for (int i = 0; i < 1000000; ++i)
            container.push_back(rand());
    }
};

BENCHMARK_PRESET(ContainerPreset<std::list<int>>, "std::list<int>-forward", iterations)
{
    for (auto it = container.begin(); it != container.end(); ++it)
        ++(*it);
}

BENCHMARK_PRESET(ContainerPreset<std::list<int>>, "std::list<int>-backward", iterations)
{
    for (auto it = container.rbegin(); it != container.rend(); ++it)
        ++(*it);
}

BENCHMARK_PRESET(ContainerPreset<std::vector<int>>, "std::vector<int>-forward", iterations)
{
    for (auto it = container.begin(); it != container.end(); ++it)
        ++(*it);
}

BENCHMARK_PRESET(ContainerPreset<std::vector<int>>, "std::vector<int>-backward", iterations)
{
    for (auto it = container.rbegin(); it != container.rend(); ++it)
        ++(*it);
}

BENCHMARK_MAIN()
