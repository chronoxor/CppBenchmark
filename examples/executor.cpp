//
// Created by Ivan Shynkarenka on 03.09.2015
//

#include "benchmark/cppbenchmark.h"

#include <chrono>
#include <thread>
#include <vector>

const int THREADS = 8;

void init()
{
    auto benchmark = BENCHCODE_SCOPE("Initialization");

    std::this_thread::sleep_for(std::chrono::seconds(2));
}

void calculate()
{
    auto benchmark = BENCHCODE_SCOPE("Calculate");

    for (int i = 0; i < 5; ++i)
    {
        auto phase1 = benchmark->StartPhase("Calculate.1");

        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        phase1->StopPhase();
    }

    auto phase2 = benchmark->StartPhase("Calculate.2");
    {
        auto phase21 = benchmark->StartPhase("Calculate.2.1");

        std::this_thread::sleep_for(std::chrono::milliseconds(200));

        phase21->StopPhase();

        auto phase22 = benchmark->StartPhase("Calculate.2.2");

        std::this_thread::sleep_for(std::chrono::milliseconds(300));

        phase22->StopPhase();
    }
    phase2->StopPhase();

    for (int i = 0; i < 3; ++i)
    {
        auto phase3 = benchmark->StartPhase("Calculate.3");

        std::this_thread::sleep_for(std::chrono::milliseconds(400));

        phase3->StopPhase();
    }
}

void cleanup()
{
    BENCHCODE_START("Cleanup");

    std::this_thread::sleep_for(std::chrono::seconds(1));

    BENCHCODE_STOP("Cleanup");
}

int main(int argc, char** argv)
{
    // Initialization
    init();

    // Start parallel calculations
    std::vector<std::thread> threads;
    for (int i = 0; i < THREADS; ++i)
        threads.emplace_back(calculate);

    // Wait for all threads
    for (auto& thread : threads)
        thread.join();

    // Cleanup
    cleanup();

    // Report benchmark results
    BENCHCODE_REPORT();

    return 0;
}
