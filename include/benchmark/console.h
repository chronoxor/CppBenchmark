/*!
    \file console.h
    \brief Console management definition
    \author Ivan Shynkarenka
    \date 29.07.2015
    \copyright MIT License
*/

#ifndef CPPBENCHMARK_CONSOLE_H
#define CPPBENCHMARK_CONSOLE_H

#include <ostream>

namespace CppBenchmark {

//! Supported console colors
enum class Color
{
    BLACK,          //!< Black color
    BLUE,           //!< Blue color
    GREEN,          //!< Green color
    CYAN,           //!< Cyan color
    RED,            //!< Red color
    MAGENTA,        //!< Magenta color
    BROWN,          //!< Brown color
    GREY,           //!< Grey color
    DARKGREY,       //!< Dark grey color
    LIGHTBLUE,      //!< Light blue color
    LIGHTGREEN,     //!< Light green color
    LIGHTCYAN,      //!< Light cyan color
    LIGHTRED,       //!< Light red color
    LIGHTMAGENTA,   //!< Light magenta color
    YELLOW,         //!< Yellow color
    WHITE           //!< White color
};

//! Stream manipulator: change console text color
/*!
    \param stream - Output stream
    \param color - Console text color
    \return Output stream
*/
template <class TOutputStream>
TOutputStream& operator<<(TOutputStream& stream, Color color);

//! Stream manipulator: change console text and background colors
/*!
    \param stream - Output stream
    \param colors - Console text and background colors
    \return Output stream
*/
template <class TOutputStream>
TOutputStream& operator<<(TOutputStream& stream, std::pair<Color, Color> colors);

//! Console management static class
/*!
    Provides console management functionality such as setting
    text and background colors.
*/
class Console
{
public:
    Console() = delete;
    Console(const Console&) = delete;
    Console(Console&&) = delete;
    ~Console() = delete;

    Console& operator=(const Console&) = delete;
    Console& operator=(Console&&) = delete;

    //! Set console text color
    /*!
        \param color - Console text color
        \param background - Console background color (default is Color::BLACK)
    */
    static void SetColor(Color color, Color background = Color::BLACK);
};

} // namespace CppBenchmark

#include "console.inl"

#endif // CPPBENCHMARK_CONSOLE_H
