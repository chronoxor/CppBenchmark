//
// Created by Ivan Shynkarenka on 03.08.2015
//

#include "benchmark/cppbenchmark.h"

#include <list>
#include <vector>

template <typename T>
class ContainerFixture
{
protected:
    T container;

    ContainerFixture()
    {
        for (int i = 0; i < 1000000; ++i)
            container.push_back(rand());
    }
};

BENCHMARK_FIXTURE(ContainerFixture<std::list<int>>, "std::list<int>-forward")
{
    for (auto it = container.begin(); it != container.end(); ++it)
        ++(*it);
}

BENCHMARK_FIXTURE(ContainerFixture<std::list<int>>, "std::list<int>-backward")
{
    for (auto it = container.rbegin(); it != container.rend(); ++it)
        ++(*it);
}

BENCHMARK_FIXTURE(ContainerFixture<std::vector<int>>, "std::vector<int>-forward")
{
    for (auto it = container.begin(); it != container.end(); ++it)
        ++(*it);
}

BENCHMARK_FIXTURE(ContainerFixture<std::vector<int>>, "std::vector<int>-backward")
{
    for (auto it = container.rbegin(); it != container.rend(); ++it)
        ++(*it);
}

BENCHMARK_MAIN()
