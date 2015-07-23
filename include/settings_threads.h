//
// Created by Ivan Shynkarenka on 21.07.2015.
//

#ifndef CPPBENCHMARK_SETTINGS_THREADS_H
#define CPPBENCHMARK_SETTINGS_THREADS_H

#include "settings.h"
#include "system.h"

namespace CppBenchmark {

class SettingsThreads : public Settings
{
public:
    SettingsThreads(int threads = System::CpuPhysicalCores()) : Settings() { Threads(threads); }
    SettingsThreads(int threads, int param) : SettingsThreads(threads) { Param(param); }
    SettingsThreads(int threads, int param1, int param2) : SettingsThreads(threads) { Pair(param1, param2); }
    SettingsThreads(int threads, int param1, int param2, int param3) : SettingsThreads(threads) { Triple(param1, param2, param3); }
    SettingsThreads(const Settings& settings) : Settings(settings) {}
    SettingsThreads(const SettingsThreads&) = default;
    SettingsThreads(SettingsThreads&&) = default;
    ~SettingsThreads() = default;

    SettingsThreads& operator=(const SettingsThreads&) = default;
    SettingsThreads& operator=(SettingsThreads&&) = default;
};

} // namespace CppBenchmark

#endif // CPPBENCHMARK_SETTINGS_THREADS_H
