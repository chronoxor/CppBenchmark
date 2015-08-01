/*!
    \file environment.cpp
    \brief Environment management static class implementation
    \author Ivan Shynkarenka
    \date 09.07.2015
    \copyright MIT License
*/

#include "environment.h"

#include <chrono>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#elif defined(linux) || defined(__linux) || defined(__linux__)
#include <sys/stat.h>
#endif

namespace CppBenchmark {

bool Environment::Is32BitOS()
{
    return !Is64BitOS();
}

bool Environment::Is64BitOS()
{
#if defined(_WIN32) || defined(_WIN64)
#if defined(_WIN64)
    return true;
#elif defined(_WIN32)
    BOOL bWow64Process = FALSE;
    return IsWow64Process(GetCurrentProcess(), &bWow64Process) && bWow64Process;
#endif
#elif defined(linux) || defined(__linux) || defined(__linux__)
    struct stat buffer;
    return (stat("/lib64/ld-linux-x86-64.so.2", &buffer) == 0);
#endif
}

bool Environment::Is32BitProcess()
{
    return !Is64BitProcess();
}

bool Environment::Is64BitProcess()
{
#if defined(_WIN32) || defined(_WIN64)
#if defined(_WIN64)
    return true;
#elif defined(_WIN32)
    return false;
#endif
#elif defined(unix) || defined(__unix) || defined(__unix__)
#if defined(__x86_64__) || defined(__ppc64__)
    return true;
#else
    return false;
#endif
#endif
}

bool Environment::IsDebug()
{
    return !IsRelease();
}

bool Environment::IsRelease()
{
#if defined(NDEBUG)
    return true;
#else
    return false;
#endif
}

time_t Environment::Timestamp()
{
    return std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
}

} // namespace CppBenchmark
