/*!
    \file settings_threads.h
    \brief Threads benchmark settings definition
    \author Ivan Shynkarenka
    \date 21.07.2015
    \copyright MIT License
*/

#ifndef CPPBENCHMARK_SETTINGS_THREADS_H
#define CPPBENCHMARK_SETTINGS_THREADS_H

#include "settings.h"
#include "system.h"

namespace CppBenchmark {

//! Threads benchmark settings
/*!
    Constructs benchmark settings by providing count of iterations and count of running threads.
*/
class SettingsThreads : public Settings
{
public:
    //! Initialize settings with count of iterations
    SettingsThreads(int iterations = 1) : Settings(iterations) {}
    //! Initialize settings with count of iterations and count of running threads
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
