//
// Created by Ivan Shynkarenka on 18.07.2015.
//

#include "macros.h"

#include <deque>
#include <list>
#include <vector>

BENCHMARK("std::list<int>.push_back", 10000000)
{
    std::list<int> l;
    for (int i = 0; i < context.x(); ++i) {
        l.push_back(i);
        context.metrics().AddItems(1);
    }
}

BENCHMARK("std::vector<int>.push_back", 10000000)
{
    std::vector<int> v;
    for (int i = 0; i < context.x(); ++i) {
        v.push_back(i);
        context.metrics().AddItems(1);
    }
}

BENCHMARK("std::deque<int>.push_back", 10000000)
{
    std::deque<int> d;
    for (int i = 0; i < context.x(); ++i) {
        d.push_back(i);
        context.metrics().AddItems(1);
    }
}

BENCHMARK_MAIN()
