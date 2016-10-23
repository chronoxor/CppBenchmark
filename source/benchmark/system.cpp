/*!
    \file system.cpp
    \brief System management implementation
    \author Ivan Shynkarenka
    \date 06.07.2015
    \copyright MIT License
*/

#include "benchmark/system.h"

#if defined(__APPLE__)
#include <mach/mach.h>
#include <mach/mach_time.h>
#include <sys/sysctl.h>
#include <math.h>
#include <pthread.h>
#elif defined(unix) || defined(__unix) || defined(__unix__)
#include <pthread.h>
#include <unistd.h>
#include <fstream>
#include <regex>
#endif
#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#include <memory>
#endif

namespace CppBenchmark {

//! @cond INTERNALS
namespace Internals {

#if defined(__APPLE__)

uint32_t CeilLog2(uint32_t x)
{
    uint32_t result = 0;

    --x;
    while (x > 0)
    {
        ++result;
        x >>= 1;
    }

    return result;
}

// This function returns the rational number inside the given interval with
// the smallest denominator (and smallest numerator breaks ties; correctness
// proof neglects floating-point errors).
mach_timebase_info_data_t BestFrac(double a, double b)
{
    if (floor(a) < floor(b))
    {
        mach_timebase_info_data_t rv = { (uint32_t)ceil(a), 1 };
        return rv;
    }

    double m = floor(a);
    mach_timebase_info_data_t next = BestFrac(1 / (b - m), 1 / (a - m));
    mach_timebase_info_data_t rv = { (int)m * next.numer + next.denom, next.numer };
    return rv;
}

// This is just less than the smallest thing we can multiply numer by without
// overflowing. CeilLog2(numer) = 64 - number of leading zeros of numer
uint64_t GetExpressibleSpan(uint32_t numer, uint32_t denom)
{
  uint64_t maxDiffWithoutOverflow = ((uint64_t)1 << (64 - CeilLog2(numer))) - 1;
  return maxDiffWithoutOverflow * numer / denom;
}

// The clock may run up to 0.1% faster or slower than the "exact" tick count.
// However, although the bound on the error is the same as for the pragmatic
// answer, the error is actually minimized over the given accuracy bound.
uint64_t PrepareTimebaseInfo(mach_timebase_info_data_t& tb)
{
    tb.numer = 0;
    tb.denom = 1;

    kern_return_t mtiStatus = mach_timebase_info(&tb);
    if (mtiStatus != KERN_SUCCESS)
        return 0;

    double frac = (double)tb.numer / tb.denom;
    uint64_t spanTarget = 315360000000000000llu;
    if (GetExpressibleSpan(tb.numer, tb.denom) >= spanTarget)
        return 0;

    for (double errorTarget = 1 / 1024.0; errorTarget > 0.000001;)
    {
        mach_timebase_info_data_t newFrac = BestFrac((1 - errorTarget) * frac, (1 + errorTarget) * frac);
        if (GetExpressibleSpan(newFrac.numer, newFrac.denom) < spanTarget)
            break;
        tb = newFrac;
        errorTarget = fabs((double)tb.numer / tb.denom - frac) / frac / 8;
    }

    return mach_absolute_time();
}

#endif

#if defined(_WIN32) || defined(_WIN64)

// Helper function to count set bits in the processor mask
DWORD CountSetBits(ULONG_PTR pBitMask)
{
    DWORD dwLeftShift = sizeof(ULONG_PTR) * 8 - 1;
    DWORD dwBitSetCount = 0;
    ULONG_PTR pBitTest = (ULONG_PTR)1 << dwLeftShift;

    for (DWORD i = 0; i <= dwLeftShift; ++i)
    {
        dwBitSetCount += ((pBitMask & pBitTest) ? 1 : 0);
        pBitTest /= 2;
    }

    return dwBitSetCount;
}

#endif

} // namespace Internals
//! @endcond

std::string System::CpuArchitecture()
{
#if defined(__APPLE__)
    char result[1024];
    size_t size = sizeof(result);
    if (sysctlbyname("machdep.cpu.brand_string", result, &size, nullptr, 0) == 0)
        return result;

    return "<unknown>";
#elif defined(unix) || defined(__unix) || defined(__unix__)
    static std::regex pattern("model name(.*): (.*)");

    std::string line;
    std::ifstream stream("/proc/cpuinfo");
    while (getline(stream, line))
    {
        std::smatch matches;
        if (std::regex_match(line, matches, pattern))
            return matches[2];
    }

    return "<unknown>";
#elif defined(_WIN32) || defined(_WIN64)
    HKEY hKeyProcessor;
    LONG lError = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", 0, KEY_READ, &hKeyProcessor);
    if (lError != ERROR_SUCCESS)
        return "<unknown>";

    // Smart resource cleaner pattern
    auto clearer = [](HKEY hKey) { RegCloseKey(hKey); };
    auto key = std::unique_ptr<std::remove_pointer<HKEY>::type, decltype(clearer)>(hKeyProcessor, clearer);

    CHAR pBuffer[_MAX_PATH] = { 0 };
    DWORD dwBufferSize = sizeof(pBuffer);
    lError = RegQueryValueExA(key.get(), "ProcessorNameString", nullptr, nullptr, (LPBYTE)pBuffer, &dwBufferSize);
    if (lError != ERROR_SUCCESS)
        return "<unknown>";

    return std::string(pBuffer);
#else
    #error Unsupported platform
#endif
}

int System::CpuLogicalCores()
{
    return CpuTotalCores().first;
}

int System::CpuPhysicalCores()
{
    return CpuTotalCores().second;
}

std::pair<int, int> System::CpuTotalCores()
{
#if defined(__APPLE__)
    int logical = 0;
    size_t logical_size = sizeof(logical);
    if (sysctlbyname("hw.logicalcpu", &logical, &logical_size, nullptr, 0) != 0)
        logical = -1;

    int physical = 0;
    size_t physical_size = sizeof(physical);
    if (sysctlbyname("hw.physicalcpu", &physical, &physical_size, nullptr, 0) != 0)
        physical = -1;

    return std::make_pair(logical, physical);
#elif defined(unix) || defined(__unix) || defined(__unix__)
    long processors = sysconf(_SC_NPROCESSORS_ONLN);
    return std::make_pair(processors, processors);
#elif defined(_WIN32) || defined(_WIN64)
    BOOL allocated = FALSE;
    PSYSTEM_LOGICAL_PROCESSOR_INFORMATION pBuffer = nullptr;
    DWORD dwLength = 0;

    while (!allocated)
    {
        DWORD dwResult = GetLogicalProcessorInformation(pBuffer, &dwLength);
        if (dwResult == FALSE)
        {
            if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
            {
                if (pBuffer != nullptr)
                    free(pBuffer);
                pBuffer = (PSYSTEM_LOGICAL_PROCESSOR_INFORMATION)malloc(dwLength);
                if (pBuffer == nullptr)
                    return std::make_pair(-1, -1);
            }
            else
                return std::make_pair(-1, -1);
        }
        else
            allocated = TRUE;
    }

    std::pair<int, int> result(0, 0);
    PSYSTEM_LOGICAL_PROCESSOR_INFORMATION pCurrent = pBuffer;
    DWORD dwOffset = 0;

    while (dwOffset + sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION) <= dwLength)
    {
        switch (pCurrent->Relationship)
        {
            case RelationProcessorCore:
                result.first += Internals::CountSetBits(pCurrent->ProcessorMask);
                result.second += 1;
                break;
            case RelationNumaNode:
            case RelationCache:
            case RelationProcessorPackage:
                break;
            default:
                return std::make_pair(-1, -1);
        }
        dwOffset += sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION);
        pCurrent++;
    }

    free(pBuffer);

    return result;
#else
    #error Unsupported platform
#endif
}

int64_t System::CpuClockSpeed()
{
#if defined(__APPLE__)
    uint64_t frequency = 0;
    size_t size = sizeof(frequency);
    if (sysctlbyname("hw.cpufrequency", &frequency, &size, nullptr, 0) == 0)
        return frequency;

    return -1;
#elif defined(unix) || defined(__unix) || defined(__unix__)
    static std::regex pattern("cpu MHz(.*): (.*)");

    std::string line;
    std::ifstream stream("/proc/cpuinfo");
    while (getline(stream, line))
    {
        std::smatch matches;
        if (std::regex_match(line, matches, pattern))
            return (int64_t)(atof(matches[2].str().c_str()) * 1000 * 1000);
    }

    return -1;
#elif defined(_WIN32) || defined(_WIN64)
    HKEY hKeyProcessor;
    long lError = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", 0, KEY_READ, &hKeyProcessor);
    if (lError != ERROR_SUCCESS)
        return -1;

    // Smart resource cleaner pattern
    auto clearer = [](HKEY hKey) { RegCloseKey(hKey); };
    auto key = std::unique_ptr<std::remove_pointer<HKEY>::type, decltype(clearer)>(hKeyProcessor, clearer);

    DWORD dwMHz = 0;
    DWORD dwBufferSize = sizeof(DWORD);
    lError = RegQueryValueExA(key.get(), "~MHz", nullptr, nullptr, (LPBYTE)&dwMHz, &dwBufferSize);
    if (lError != ERROR_SUCCESS)
        return -1;

    return dwMHz * 1000 * 1000;
#else
    #error Unsupported platform
#endif
}

bool System::CpuHyperThreading()
{
    std::pair<int, int> cores = CpuTotalCores();
    return (cores.first != cores.second);
}

int64_t System::RamTotal()
{
#if defined(__APPLE__)
    int64_t memsize = 0;
    size_t size = sizeof(memsize);
    if (sysctlbyname("hw.memsize", &memsize, &size, nullptr, 0) == 0)
        return memsize;

    return -1;
#elif defined(unix) || defined(__unix) || defined(__unix__)
    int64_t pages = sysconf(_SC_PHYS_PAGES);
    int64_t page_size = sysconf(_SC_PAGESIZE);
    if ((pages > 0) && (page_size > 0))
        return pages * page_size;

    return -1;
#elif defined(_WIN32) || defined(_WIN64)
    MEMORYSTATUSEX status;
    status.dwLength = sizeof(status);
    GlobalMemoryStatusEx(&status);
    return status.ullTotalPhys;
#else
    #error Unsupported platform
#endif
}

int64_t System::RamFree()
{
#if defined(__APPLE__)
    mach_port_t host_port = mach_host_self();
    if (host_port == MACH_PORT_NULL)
        return -1;

    vm_size_t page_size = 0;
    host_page_size(host_port, &page_size);

    vm_statistics_data_t vmstat;
    mach_msg_type_number_t count = HOST_VM_INFO_COUNT;
    kern_return_t kernReturn = host_statistics(host_port, HOST_VM_INFO, (host_info_t)&vmstat, &count);
    if (kernReturn != KERN_SUCCESS)
        return -1;

    int64_t used_mem = (vmstat.active_count + vmstat.inactive_count + vmstat.wire_count) * page_size;
    int64_t free_mem = vmstat.free_count * page_size;
    (void)used_mem;
    return free_mem;
#elif defined(unix) || defined(__unix) || defined(__unix__)
    int64_t pages = sysconf(_SC_AVPHYS_PAGES);
    int64_t page_size = sysconf(_SC_PAGESIZE);
    if ((pages > 0) && (page_size > 0))
        return pages * page_size;

    return -1;
#elif defined(_WIN32) || defined(_WIN64)
    MEMORYSTATUSEX status;
    status.dwLength = sizeof(status);
    GlobalMemoryStatusEx(&status);
    return status.ullAvailPhys;
#else
    #error Unsupported platform
#endif
}

uint64_t System::CurrentThreadId()
{
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
    return (uint64_t)pthread_self();
#elif defined(_WIN32) || defined(_WIN64)
    return GetCurrentThreadId();
#else
    #error Unsupported platform
#endif
}

uint64_t System::Timestamp()
{
#if defined(__APPLE__)
    static mach_timebase_info_data_t info;
    static uint64_t bias = Internals::PrepareTimebaseInfo(info);
    return (mach_absolute_time() - bias) * info.numer / info.denom;
#elif defined(unix) || defined(__unix) || defined(__unix__)
    struct timespec timestamp = { 0 };
    clock_gettime(CLOCK_MONOTONIC, &timestamp);
    return (timestamp.tv_sec * 1000 * 1000 * 1000) + timestamp.tv_nsec;
#elif defined(_WIN32) || defined(_WIN64)
    static uint64_t offset = 0;
    static LARGE_INTEGER first = { 0 };
    static LARGE_INTEGER frequency = { 0 };
    static bool initialized = false;
    static bool qpc = true;

    if (!initialized)
    {
        // Calculate timestamp offset
        FILETIME timestamp;
        GetSystemTimePreciseAsFileTime(&timestamp);

        ULARGE_INTEGER result;
        result.LowPart = timestamp.dwLowDateTime;
        result.HighPart = timestamp.dwHighDateTime;

        // Convert 01.01.1601 to 01.01.1970
        result.QuadPart -= 116444736000000000ll;
        offset = result.QuadPart * 100;

        // Setup performance counter
        qpc = QueryPerformanceFrequency(&frequency) && QueryPerformanceCounter(&first);

        initialized = true;
    }

    if (qpc)
    {
        LARGE_INTEGER timestamp = { 0 };
        QueryPerformanceCounter(&timestamp);
        timestamp.QuadPart -= first.QuadPart;
        return offset + ((timestamp.QuadPart * 1000 * 1000 * 1000) / frequency.QuadPart);
    }
    else
        return offset;
#else
    #error Unsupported platform
#endif
}

} // namespace CppBenchmark
