//
// Created by Ivan Shynkarenka on 29.07.2015.
//

#ifndef CPPBENCHMARK_CONSOLE_H
#define CPPBENCHMARK_CONSOLE_H

#include <ostream>

namespace CppBenchmark {

enum Color
{
    BLACK,
    BLUE,
    GREEN,
    CYAN,
    RED,
    MAGENTA,
    BROWN,
    GREY,
    DARKGREY,
    LIGHTBLUE,
    LIGHTGREEN,
    LIGHTCYAN,
    LIGHTRED,
    LIGHTMAGENTA,
    YELLOW,
    WHITE
};

std::ostream& operator<<(std::ostream& stream, Color color);

class Console
{
public:
    Console() = delete;
    Console(const Console&) = delete;
    Console(Console&&) = delete;
    ~Console() = delete;

    Console& operator=(const Console&) = delete;
    Console& operator=(Console&&) = delete;

    static void SetColor(Color color);
};

} // namespace CppBenchmark

#endif // CPPBENCHMARK_CONSOLE_H
