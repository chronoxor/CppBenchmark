//
// Created by Ivan Shynkarenka on 06.07.2015.
//

#ifndef CPPBENCHMARK_SYSTEM_H
#define CPPBENCHMARK_SYSTEM_H

#include <string>

namespace CppBenchmark {

class System
{
public:
    System() = delete;
    System(const System&) = delete;
    System(System&&) = delete;
    ~System() = delete;

    System& operator=(const System&) = delete;
    System& operator=(System&&) = delete;

    static std::string CpuArchitecture();
    static int CpuLogicalCores();
    static int CpuPhysicalCores();
    static std::pair<int, int> CpuTotalCores();
    static int64_t CpuClockSpeed();
    static bool CpuHyperThreading();

    static int64_t RamTotal();
    static int64_t RamFree();

    static int CurrentThreadId();
};

} // namespace CppBenchmark

#endif // CPPBENCHMARK_SYSTEM_H
