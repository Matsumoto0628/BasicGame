#include "string_converter.h"

std::wstring StringToWString(const std::string& str)
{
    if (str.empty())
        return L"";

    int len = MultiByteToWideChar(
        CP_UTF8,
        0,
        str.c_str(),
        -1,
        nullptr,
        0
    );

    std::wstring result(len, L'\0');

    MultiByteToWideChar(
        CP_UTF8,
        0,
        str.c_str(),
        -1,
        &result[0],
        len
    );

    // nullI’[‚ğíœ
    result.pop_back();

    return result;
}