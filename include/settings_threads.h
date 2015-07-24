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
    SettingsThreads(int iterations = 1) : Settings(iterations) {}
    SettingsThreads(int iterations, int threads) : SettingsThreads(iterations) { Threads(threads); }
    SettingsThreads(const Settings& settings) : Settings(settings) {}	
    SettingsThreads(const SettingsThreads&) = default;
    SettingsThreads(Settings&& settings) : Settings(settings) {}	
    SettingsThreads(SettingsThreads&&) = default;
    ~SettingsThreads() = default;

    SettingsThreads& operator=(const SettingsThreads&) = default;
    SettingsThreads& operator=(SettingsThreads&&) = default;
};

} // namespace CppBenchmark

#endif // CPPBENCHMARK_SETTINGS_THREADS_H
