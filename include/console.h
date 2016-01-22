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
enum Color
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
std::ostream& operator<<(std::ostream& stream, Color color);

//! Console management static class
/*!
    Provides console management functionality such as setting text color.
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
    */
    static void SetColor(Color color);
};

} // namespace CppBenchmark

#endif // CPPBENCHMARK_CONSOLE_H
