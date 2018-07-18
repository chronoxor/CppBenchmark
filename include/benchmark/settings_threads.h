/*!
    \file settings_threads.h
    \brief Threads benchmark settings definition
    \author Ivan Shynkarenka
    \date 21.07.2015
    \copyright MIT License
*/

#ifndef CPPBENCHMARK_SETTINGS_THREADS_H
#define CPPBENCHMARK_SETTINGS_THREADS_H

#include "benchmark/settings.h"

namespace CppBenchmark {

//! Threads benchmark settings
/*!
    Constructs benchmark settings by providing count of operations and count of running threads.
*/
class SettingsThreads : public Settings
{
public:
    //! Initialize settings with the default benchmark duration (5 seconds)
    SettingsThreads() : Settings() {}
    //! Initialize settings with the given count of running threads
    /*!
        \param threads - Count of running threads
    */
    SettingsThreads(int threads) : Settings() { Threads(threads); }
    //! Initialize settings with the given count of running threads and the given count of operations
    /*!
        \param threads - Count of running threads
        \param operations - Count of operations
    */
    SettingsThreads(int threads, int64_t operations) : Settings(operations) { Threads(threads); }
    SettingsThreads(const Settings& settings) : Settings(settings) {}
    SettingsThreads(const SettingsThreads&) = default;
    SettingsThreads(Settings&& settings) noexcept : Settings(std::move(settings)) {}
    SettingsThreads(SettingsThreads&&) noexcept = default;
    ~SettingsThreads() = default;

    SettingsThreads& operator=(const SettingsThreads&) = default;
    SettingsThreads& operator=(SettingsThreads&&) noexcept = default;
};

} // namespace CppBenchmark

#endif // CPPBENCHMARK_SETTINGS_THREADS_H
