/*!
    \file barrier.cpp
    \brief Barrier synchronization primitive implementation
    \author Ivan Shynkarenka
    \date 04.04.2016
    \copyright MIT License
*/

#include "benchmark/barrier.h"

#include <cassert>

namespace CppBenchmark {

Barrier::Barrier(int threads) noexcept : _counter(threads), _generation(0), _threads(threads)
{
    assert((threads > 0) && "Barrier threads counter must be greater than zero!");
}

bool Barrier::Wait() noexcept
{
    std::unique_lock<std::mutex> lock(_mutex);

    // Remember the current barrier generation
    int generation = _generation;

    // Decrease the count of waiting threads
    if (--_counter == 0)
    {
        // Increase the current barrier generation
        ++_generation;

        // Reset waiting threads counter
        _counter = _threads;

        // Notify all waiting threads
        _cond.notify_all();

        // Notify the last thread that reached the barrier
        return true;
    }

    // Wait for the next barrier generation
    _cond.wait(lock, [&, this]() { return generation != _generation; });

    // Notify each of remaining threads
    return false;
}

} // namespace CppBenchmark
