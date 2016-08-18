/*!
    \file encoding.cpp
    \brief Encoding utilities implementation
    \author Ivan Shynkarenka
    \date 12.08.2016
    \copyright MIT License
*/

#include "benchmark/encoding.h"

#include <codecvt>
#include <locale>

namespace CppBenchmark {

std::string Encoding::ToUTF8(const std::wstring& wstr)
{
#if defined(_WIN32) || defined(_WIN64)
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> convert;
    return convert.to_bytes(wstr);
#elif defined(unix) || defined(__unix) || defined(__unix__)
    std::wstring_convert<std::codecvt_utf8<wchar_t>> convert;
    return convert.to_bytes(wstr);
#endif
}

std::wstring Encoding::FromUTF8(const std::string& str)
{
#if defined(_WIN32) || defined(_WIN64)
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> convert;
    return convert.from_bytes(str);
#elif defined(unix) || defined(__unix) || defined(__unix__)
    std::wstring_convert<std::codecvt_utf8<wchar_t>> convert;
    return convert.from_bytes(str);
#endif
}

std::u16string Encoding::UTF8toUTF16(const std::string& str)
{
#if defined(_MSC_VER) && (_MSC_VER == 1900)
    std::wstring_convert<std::codecvt_utf8_utf16<uint16_t>, uint16_t> convert;
    auto tmp = convert.from_bytes(str);
    return std::u16string(tmp.data(), tmp.data() + tmp.size());
#else
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
    return convert.from_bytes(str);
#endif
}

std::u32string Encoding::UTF8toUTF32(const std::string& str)
{
#if defined(_MSC_VER) && (_MSC_VER == 1900)
    std::wstring_convert<std::codecvt_utf8<uint32_t>, uint32_t> convert;
    auto tmp = convert.from_bytes(str);
    return std::u32string(tmp.data(), tmp.data() + tmp.size());
#else
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> convert;
    return convert.from_bytes(str);
#endif
}

std::string Encoding::UTF16toUTF8(const std::u16string& str)
{
#if defined(_MSC_VER) && (_MSC_VER == 1900)
    std::wstring_convert<std::codecvt_utf8_utf16<uint16_t>, uint16_t> convert;
    return convert.to_bytes((uint16_t*)str.data(), (uint16_t*)str.data() + str.size());
#else
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
    return convert.to_bytes(str);
#endif
}

std::u32string Encoding::UTF16toUTF32(const std::u16string& str)
{
    std::string bytes;
    bytes.reserve(str.size() * 2);

    for (const char16_t ch : str)
    {
        bytes.push_back((uint8_t)(ch / 256));
        bytes.push_back((uint8_t)(ch % 256));
    }

#if defined(_MSC_VER) && (_MSC_VER == 1900)
    std::wstring_convert<std::codecvt_utf16<uint32_t>, uint32_t> convert;
    auto tmp = convert.from_bytes(bytes);
    return std::u32string(tmp.data(), tmp.data() + tmp.size());
#else
    std::wstring_convert<std::codecvt_utf16<char32_t>, char32_t> convert;
    return convert.from_bytes(bytes);
#endif
}

std::string Encoding::UTF32toUTF8(const std::u32string& str)
{
#if defined(_MSC_VER) && (_MSC_VER == 1900)
    std::wstring_convert<std::codecvt_utf8<uint32_t>, uint32_t> convert;
    return convert.to_bytes((uint32_t*)str.data(), (uint32_t*)str.data() + str.size());
#else
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> convert;
    return convert.to_bytes(str);
#endif
}

std::u16string Encoding::UTF32toUTF16(const std::u32string& str)
{
#if defined(_MSC_VER) && (_MSC_VER == 1900)
    std::wstring_convert<std::codecvt_utf16<uint32_t>, uint32_t> convert;
    std::string bytes = convert.to_bytes((uint32_t*)str.data(), (uint32_t*)str.data() + str.size());
#else
    std::wstring_convert<std::codecvt_utf16<char32_t>, char32_t> convert;
    std::string bytes = convert.to_bytes(str);
#endif

    std::u16string result;
    result.reserve(bytes.size() / 2);

    for (size_t i = 0; i < bytes.size(); i += 2)
        result.push_back((char16_t)((uint8_t)(bytes[i]) * 256 + (uint8_t)(bytes[i + 1])));

    return result;
}

} // namespace CppBenchmark
