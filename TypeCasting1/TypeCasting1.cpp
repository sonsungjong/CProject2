#include "pch.h"
#include "framework.h"
#include "TypeCasting1.h"

#include <locale>
#include <string>
#include <sstream>
#include <vector>

CWinApp theApp;
typedef std::basic_string<TCHAR> TSTRING;
typedef std::basic_stringstream<TCHAR> Tstringstream;
#define SIZE256           256

// int -> string
std::string IntToString(int num)
{
    std::stringstream local_str;
    local_str << num;
    return local_str.str();
}

// int -> wstring
TSTRING IntToTstr(int num)
{
    Tstringstream local_tstr;
    local_tstr << num;
    return local_tstr.str();
}

// string -> int
int TstrToInt(TCHAR* a_tstr)
{
    return _ttoi(a_tstr);
}

// TSTRING -> TCHAR*
TCHAR* TstrToTchar(TSTRING a_str)
{
    std::vector<TCHAR> tstr_cast(a_str.begin(), a_str.end());
    tstr_cast.push_back('\0');
    TCHAR* result = &tstr_cast[0];
    return result;
    //return (a_str.c_str());
}

// TCHAR* -> TSTRING
TSTRING TcharToTstring(TCHAR* a_lptstr)
{
    TSTRING local_tstr(a_lptstr);
    return local_tstr;
}

// TCHAR[] -> TSTRING

// wstring -> CString
CString TstrToCString(TSTRING a_tstr)
{
    return a_tstr.c_str();
}

// CString -> TSTRING
TSTRING CStringToTstr(CString a_cstring)
{
    TSTRING local_tstr = a_cstring.operator LPCTSTR();
    return local_tstr;
}

// CString -> string
std::string CStringToStr(CString a_cstring)
{
    std::string local_str = "";
    std::wstring local_wstr = a_cstring.operator LPCWSTR();
    local_str.assign(local_wstr.begin(), local_wstr.end());
    return local_str;
}

// CString -> TCHAR[]
void CStringToTchar(CString a_cstring, TCHAR* a_tchar, const int a_size)
{
    memset(a_tchar, 0x00, sizeof(TCHAR) * a_size);

    _tcscpy_s(a_tchar, a_size, a_cstring.GetBuffer(0));
    a_cstring.ReleaseBuffer();
}

// wstring -> string
std::string WstrToStr(std::wstring a_wstr)
{
    std::string local_str = "";
    local_str.assign(a_wstr.begin(), a_wstr.end());
    return local_str;
}

// string -> wstring
std::wstring StrToWstr(std::string a_str)
{
    std::wstring local_wstr = L"";
    local_wstr.assign(a_str.begin(), a_str.end());
    return local_wstr;
}

// stringstream -> string
TSTRING TstreamToTstr(Tstringstream a_ss)
{
    return a_ss.str();
}

// 실수 -> 문자열

// 문자열 -> 실수

int main()
{
    _tsetlocale(0, _T("korean"));               // wchar_t 한글깨짐방지
    // 테스트
    CString cs = _T("안녕반갑aa123!@#");
    TCHAR str[128];
    int s_size = sizeof(str) / sizeof(str[0]);
    CStringToTchar(cs, str, s_size);

    _tprintf(_T("%s"), str);
}
