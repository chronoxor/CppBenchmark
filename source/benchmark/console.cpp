/*!
    \file console.cpp
    \brief Console management implementation
    \author Ivan Shynkarenka
    \date 29.07.2015
    \copyright MIT License
*/

#include "benchmark/console.h"

#include <iostream>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

namespace CppBenchmark {

std::ostream& operator<<(std::ostream& stream, Color color)
{
    Console::SetColor(color);
    return stream;
}

void Console::SetColor(Color color)
{
#if defined(_WIN32) || defined(_WIN64)
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (WORD)color);
#elif defined(unix) || defined(__unix) || defined(__unix__)
    const char* colors[] =
    {
        "\033[22;30m",  // Black color
        "\033[22;34m",  // Blue color
        "\033[22;32m",  // Green color
        "\033[22;36m",  // Cyan color
        "\033[22;31m",  // Red color
        "\033[22;35m",  // Magenta color
        "\033[22;33m",  // Brown color
        "\033[22;37m",  // Grey color
        "\033[01;30m",  // Dark grey color
        "\033[01;34m",  // Light blue color
        "\033[01;32m",  // Light green color
        "\033[01;36m",  // Light cyan color
        "\033[01;31m",  // Light red color
        "\033[01;35m",  // Light magenta color
        "\033[01;33m",  // Yellow color
        "\033[01;37m"   // White color
    };
    std::cout << colors[color - BLACK];
#endif
}

} // namespace CppBenchmark
