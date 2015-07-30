//
// Created by Ivan Shynkarenka on 24.07.2015.
//

#ifndef CPPBENCHMARK_SETTINGS_PC_H
#define CPPBENCHMARK_SETTINGS_PC_H

#include "settings_threads.h"

namespace CppBenchmark {

class SettingsPC : public SettingsThreads
{
public:
    SettingsPC(int iterations = 1) : SettingsThreads(iterations) {}
    SettingsPC(int iterations, int producers, int consumers) : SettingsPC(iterations) { PC(producers, consumers); }
    SettingsPC(const Settings& settings) : SettingsThreads(settings) {}
    SettingsPC(const SettingsPC&) = default;
    SettingsPC(Settings&& settings) : SettingsThreads(settings) {}
    SettingsPC(SettingsPC&&) = default;
    ~SettingsPC() = default;

    SettingsPC& operator=(const SettingsPC&) = default;
    SettingsPC& operator=(SettingsPC&&) = default;
};

} // namespace CppBenchmark

#endif // CPPBENCHMARK_SETTINGS_PC_H
