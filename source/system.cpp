//
// Created by Ivan Shynkarenka on 06.07.2015.
//

#include "system.h"

#ifdef _WIN32
#include <windows.h>
#endif

namespace {

#ifdef _WIN32
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

} // namespace

namespace CppBenchmark {

std::string System::CpuArchitecture()
{
#ifdef _WIN32
    HKEY hKey;
    LONG lError = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", 0, KEY_READ, &hKey);
    if (lError != ERROR_SUCCESS)
        return 0;

    CHAR pBuffer[_MAX_PATH] = { 0 };
    DWORD dwBufferSize = _MAX_PATH;
    lError = RegQueryValueEx(hKey, "ProcessorNameString", NULL, NULL, (LPBYTE)pBuffer, &dwBufferSize);
    if (lError != ERROR_SUCCESS)
        return 0;

    return std::string(pBuffer);
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
#ifdef _WIN32
    BOOL allocated = FALSE;
    PSYSTEM_LOGICAL_PROCESSOR_INFORMATION pBuffer = NULL;
    DWORD dwLength = 0;

    while (!allocated) {
        DWORD dwResult = GetLogicalProcessorInformation(pBuffer, &dwLength);
        if (dwResult == FALSE) {
            if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
                if (pBuffer != NULL)
                    free(pBuffer);
                pBuffer = (PSYSTEM_LOGICAL_PROCESSOR_INFORMATION)malloc(dwLength);
                if (pBuffer == NULL)
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
                result.first += CountSetBits(pCurrent->ProcessorMask);
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
#endif
}

int64_t System::CpuClockSpeed()
{
#ifdef _WIN32
    HKEY hKey;
    long lError = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", 0, KEY_READ, &hKey);
    if (lError != ERROR_SUCCESS)
        return -1;

    DWORD dwMHz = 0;
    DWORD dwBufferSize = sizeof(DWORD);
    lError = RegQueryValueEx(hKey, "~MHz", NULL, NULL, (LPBYTE)&dwMHz, &dwBufferSize);
    if (lError != ERROR_SUCCESS)
        return -1;

    return dwMHz * 1000 * 1000;
#endif
}

bool System::CpuHyperThreading()
{
    std::pair<int, int> cores = CpuTotalCores();
    return (cores.first != cores.second);
}

int64_t System::RamTotal()
{
#ifdef _WIN32
    MEMORYSTATUSEX status;
    status.dwLength = sizeof(status);
    GlobalMemoryStatusEx(&status);
    return status.ullTotalPhys;
#endif
}

int64_t System::RamFree()
{
#ifdef _WIN32
    MEMORYSTATUSEX status;
    status.dwLength = sizeof(status);
    GlobalMemoryStatusEx(&status);
    return status.ullAvailPhys;
#endif
}

} // namespace CppBenchmark
