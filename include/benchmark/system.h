/*!
    \file system.h
    \brief System management definition
    \author Ivan Shynkarenka
    \date 06.07.2015
    \copyright MIT License
*/

#ifndef CPPBENCHMARK_SYSTEM_H
#define CPPBENCHMARK_SYSTEM_H

#include <cstdint>
#include <string>

namespace CppBenchmark {

//! System management static class
/*!
    Provides system management functionality to get CPU properties, RAM properties, current thread Id, etc.
*/
class System
{
public:
    System() = delete;
    System(const System&) = delete;
    System(System&&) = delete;
    ~System() = delete;

    System& operator=(const System&) = delete;
    System& operator=(System&&) = delete;

    //! CPU architecture string
    static std::string CpuArchitecture();
    //! CPU logical cores count
    static int CpuLogicalCores();
    //! CPU physical cores count
    static int CpuPhysicalCores();
    //! CPU total cores count
    static std::pair<int, int> CpuTotalCores();
    //! CPU clock speed in Hz
    static int64_t CpuClockSpeed();
    //! Is CPU Hyper-Threading enabled?
    static bool CpuHyperThreading();

    //! Total RAM in bytes
    static int64_t RamTotal();
    //! Free RAM in bytes
    static int64_t RamFree();

    //! Current thread Id
    static uint64_t CurrentThreadId();

    //! Get the current timestamp in nanoseconds
    static uint64_t Timestamp();

    //! Calculate (operant * multiplier / divider) with 64-bit unsigned integer values
    static uint64_t MulDiv64(uint64_t operant, uint64_t multiplier, uint64_t divider);
};

} // namespace CppBenchmark

#endif // CPPBENCHMARK_SYSTEM_H
