/*!
    \file console.cpp
    \brief Console management static class implementation
    \author Ivan Shynkarenka
    \date 29.07.2015
    \copyright MIT License
*/

#include "console.h"

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
    const char* colors[] = {
        "\033[22;30m",
        "\033[22;31m",
        "\033[22;32m",
        "\033[22;33m",
        "\033[22;34m",
        "\033[22;35m",
        "\033[22;36m",
        "\033[22;37m",
        "\033[01;30m",
        "\033[01;31m",
        "\033[01;32m",
        "\033[01;33m",
        "\033[01;34m",
        "\033[01;35m",
        "\033[01;36m",
        "\033[01;37m"
    };
    std::cout << colors[color - BLACK];
#endif
}

} // namespace CppBenchmark
