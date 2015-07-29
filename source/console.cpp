//
// Created by Ivan Shynkarenka on 29.07.2015.
//

#include "console.h"

#include <windows.h>

namespace CppBenchmark {

std::ostream& operator<<(std::ostream& stream, Color color)
{
    Console::SetColor(color);
    return stream;
}

void Console::SetColor(Color color)
{
#if defined(_WIN32)
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (WORD)color);
#endif
}

} // namespace CppBenchmark
