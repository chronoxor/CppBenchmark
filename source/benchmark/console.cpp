/*!
    \file console.cpp
    \brief Console management implementation
    \author Ivan Shynkarenka
    \date 29.07.2015
    \copyright MIT License
*/

#include "benchmark/console.h"

#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
#include <cstdio>
#elif defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

namespace CppBenchmark {

void Console::SetColor(Color color, Color background)
{
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
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
    const char* backgrounds[] =
    {
        "\033[00000m",  // Black color
        "\033[02;44m",  // Blue color
        "\033[02;42m",  // Green color
        "\033[02;46m",  // Cyan color
        "\033[02;41m",  // Red color
        "\033[02;45m",  // Magenta color
        "\033[02;43m",  // Brown color
        "\033[02;47m",  // Grey color
        "\033[00;40m",  // Dark grey color
        "\033[00;44m",  // Light blue color
        "\033[00;42m",  // Light green color
        "\033[00;46m",  // Light cyan color
        "\033[00;41m",  // Light red color
        "\033[00;45m",  // Light magenta color
        "\033[00;43m",  // Yellow color
        "\033[00;47m"   // White color
    };
    std::fwrite(backgrounds[(int)background - (int)Color::BLACK], 1, 8, stdout);
    std::fwrite(colors[(int)color - (int)Color::BLACK], 1, 8, stdout);
#elif defined(_WIN32) || defined(_WIN64)
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (((WORD)color) & 0x0F) + ((((WORD)background) & 0x0F) << 4));
#endif
}

} // namespace CppBenchmark
