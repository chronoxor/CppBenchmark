/*!
    \file encoding.h
    \brief Encoding utilities definition
    \author Ivan Shynkarenka
    \date 12.08.2016
    \copyright MIT License
*/

#ifndef CPPBENCHMARK_ENCODING_H
#define CPPBENCHMARK_ENCODING_H

#include <string>

namespace CppBenchmark {

//! Encoding utilities
/*!
    Encoding utilities contains methods for UTF-8, UTF-16, UTF-32 encoding conversions.

    Thread-safe.
*/
class Encoding
{
public:
    Encoding() = delete;
    Encoding(const Encoding&) = delete;
    Encoding(Encoding&&) = delete;
    ~Encoding() = delete;

    Encoding& operator=(const Encoding&) = delete;
    Encoding& operator=(Encoding&&) = delete;

    //! Convert system wide-string to UTF-8 encoded string
    /*!
        System wide-string could be UTF-16 (Windows) or UTF-32 (Unix).

        \param wstr - System wide-string to convert
        \return UTF-8 encoded string
    */
    static std::string ToUTF8(const std::wstring& wstr);

    //! Convert UTF-8 encoded string to system wide-string
    /*!
        System wide-string could be UTF-16 (Windows) or UTF-32 (Unix).

        \param str - UTF-8 encoded string to convert
        \return System wide-string
    */
    static std::wstring FromUTF8(const std::string& str);

    //! Convert UTF-8 encoded string to UTF-16 encoded string
    /*!
        \param str - UTF-8 encoded string to convert
        \return UTF-16 encoded string
    */
    static std::u16string UTF8toUTF16(const std::string& str);
    //! Convert UTF-8 encoded string to UTF-32 encoded string
    /*!
        \param str - UTF-8 encoded string to convert
        \return UTF-32 encoded string
    */
    static std::u32string UTF8toUTF32(const std::string& str);

    //! Convert UTF-16 encoded string to UTF-8 encoded string
    /*!
        \param str - UTF-16 encoded string to convert
        \return UTF-8 encoded string
    */
    static std::string UTF16toUTF8(const std::u16string& str);
    //! Convert UTF-16 encoded string to UTF-32 encoded string
    /*!
        \param str - UTF-16 encoded string to convert
        \return UTF-32 encoded string
    */
    static std::u32string UTF16toUTF32(const std::u16string& str);

    //! Convert UTF-32 encoded string to UTF-8 encoded string
    /*!
        \param str - UTF-32 encoded string to convert
        \return UTF-8 encoded string
    */
    static std::string UTF32toUTF8(const std::u32string& str);
    //! Convert UTF-32 encoded string to UTF-16 encoded string
    /*!
        \param str - UTF-32 encoded string to convert
        \return UTF-16 encoded string
    */
    static std::u16string UTF32toUTF16(const std::u32string& str);
};

/*! \example string_encoding.cpp Encoding utilities example */

} // namespace CppBenchmark

#endif // CPPBENCHMARK_ENCODING_H
