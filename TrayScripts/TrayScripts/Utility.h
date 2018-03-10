#pragma once
#include <string>
#include <codecvt> 

//this localization crap is deprecated in c++17. Don't know why I'm even bothering this much to add a wide string conversion
namespace Utility
{
    std::string ws2s(const std::wstring& wstr)
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converterX;
        return converterX.to_bytes(wstr);
    }
}