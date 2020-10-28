#ifndef SCONV_STR_CODEC_HPP
#define SCONV_STR_CODEC_HPP

#include "sconv.h"
#include <string>

inline std::wstring gbk_to_unicode(const std::string& gbk_str)
{
    int size = sconv_gbk_to_unicode(gbk_str.c_str(), gbk_str.size(), 0, 0);
    wchar* unicode = new wchar[size / 2 + 1];
    size = sconv_gbk_to_unicode(gbk_str.c_str(), gbk_str.size(), unicode, sizeof(wchar) * (size / 2 + 1));
    unicode[size / 2] = 0;
    std::wstring ret((wchar_t*)unicode, size / 2);
    delete[] unicode;
    return ret;
}

inline std::string unicode_to_gbk(const std::wstring& unicode_str)
{
    int size = sconv_unicode_to_gbk((wchar *)unicode_str.c_str(), unicode_str.size(), 0, 0);
    char* gbk = new char[size + 1];
    size = sconv_unicode_to_gbk((wchar *)unicode_str.c_str(), unicode_str.size(), gbk, size + 1);
    gbk[size] = 0;
    std::string ret(gbk, size);
    delete[] gbk;
    return ret;
}

inline std::wstring utf8_to_unicode(const std::string& utf8_str)
{
    int size = sconv_utf8_to_unicode(utf8_str.c_str(), utf8_str.size(), 0, 0);
    wchar* unicode = new wchar[size / 2 + 1];
    size = sconv_utf8_to_unicode(utf8_str.c_str(), utf8_str.size(), unicode, sizeof(wchar) * (size / 2 + 1));
    unicode[size / 2] = 0;
    std::wstring ret((wchar_t*)unicode, size / 2);
    delete[] unicode;
    return ret;
}

inline std::string unicode_to_utf8(const std::wstring& unicode_str)
{
    int size = sconv_unicode_to_utf8((wchar *)unicode_str.c_str(), unicode_str.size(), 0, 0);
    char* utf8 = new char[size + 1];
    size = sconv_unicode_to_utf8((wchar *)unicode_str.c_str(), unicode_str.size(), utf8, size + 1);
    utf8[size] = 0;
    std::string ret(utf8, size);
    delete[] utf8;
    return ret;
}

inline std::string gbk_to_utf8(const std::string& gbk_str)
{
    int size = sconv_gbk_to_unicode(gbk_str.c_str(), gbk_str.size(), 0, 0);
    wchar* unicode = new wchar[size / 2 + 1];
    size = sconv_gbk_to_unicode(gbk_str.c_str(), gbk_str.size(), unicode, sizeof(wchar) * (size / 2 + 1));
    unicode[size / 2] = 0;
    int wlen = size / 2;
    size = sconv_unicode_to_utf8(unicode, wlen, 0, 0);
    char* utf8 = new char[size + 1];
    size = sconv_unicode_to_utf8(unicode, wlen, utf8, size + 1);
    utf8[size] = 0;
    std::string ret(utf8, size);
    delete[] utf8;
    delete[] unicode;
    return ret;
}

inline std::string utf8_to_gbk(const std::string& utf8_str)
{
    int size = sconv_utf8_to_unicode(utf8_str.c_str(), utf8_str.size(), 0, 0);
    wchar* unicode = new wchar[size / 2 + 1];
    size = sconv_utf8_to_unicode(utf8_str.c_str(), utf8_str.size(), unicode, sizeof(wchar) * (size / 2 + 1));
    unicode[size / 2] = 0;
    int wlen = size / 2;
    size = sconv_unicode_to_gbk(unicode, wlen, 0, 0);
    char* gbk = new char[size + 1];
    size = sconv_unicode_to_gbk(unicode, wlen, gbk, size + 1);
    gbk[size] = 0;
    std::string ret(gbk, size);
    delete[] gbk;
    delete[] unicode;
    return ret;
}

#endif