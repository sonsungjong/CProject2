/*
<라이브러리 빌드 방법(Release x64)>
Git Bash 를 설치한다.
CMake를 설치한다.
Visual Studio를 설치한다 (Visual Studio 17 2022, Visual Studio 16 2019 등)
C드라이브에 git bash를 관리자 권한으로 실행하고
git clone https://github.com/troldal/OpenXLSX.git
를 입력하여 프로젝트를 설치한다.

cd OpenXLSX
mkdir build
cd build
cmake -G "Visual Studio 17 2022" -A x64 ..
cmake --build . --config Release
cmake --install .

만약, 64비트로 빌드하고 32비트도 빌드하고자 하면 이전 빌드 파일과 CMake 캐시를 제거해야한다.
cd C:/OpenXLSX/build
rm -rf CMakeCache.txt CMakeFiles/
cmake -G "Visual Studio 17 2022" -A Win32 ..
cmake --build . --target OpenXLSX --config Release
cmake --install .

<추가사항 1>
C:\OpenXLSX\build\OpenXLSX 경로에 있는
OpenXLSX-Exports.hpp 파일을 복사하여
C:\OpenXLSX\OpenXLSX\headers 경로에 붙여넣어 줘야합니다.

<추가사항 2>
프로젝트 속성에서 C++ 언어 표준을 ISO C++17 Standard (/std:c++17) 로 변경해줘야 합니다.


<포함 헤더 디렉토리 경로>
C:\OpenXLSX

<포함 라이브러리 디렉토리 경로>
C:\OpenXLSX\build\output\Release

<포함 라이브러리>
OpenXLSX.lib
*/

#include <OpenXLSX/OpenXLSX.hpp>
#include <iostream>
#include <windows.h>
#include <tchar.h>

using namespace OpenXLSX;

std::wstring utf8_to_wstring(const std::string& utf8str)
{
    int wlen = MultiByteToWideChar(CP_UTF8, 0, utf8str.c_str(), -1, NULL, 0);
    std::wstring wstr(wlen, 0);
    MultiByteToWideChar(CP_UTF8, 0, utf8str.c_str(), -1, &wstr[0], wlen);
    return wstr;
}

std::string wstring_to_utf8(const std::wstring& wstr)
{
    int len = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
    std::string str(len, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &str[0], len, NULL, NULL);
    return str;
}

int main()
{
    _tsetlocale(0, _T(""));
    std::wstring filepath = L"C:\\csv\\신궁 군수.xlsx";
    std::wstring sheetname = L"데이터 정제";

    // 엑셀 파일 열기
    XLDocument doc;
    doc.open(wstring_to_utf8(filepath));

    // 해당 시트이름 가져오기
    XLWorksheet wks = doc.workbook().worksheet(wstring_to_utf8(sheetname));

    // 워크시트의 모든 행과 열을 순회하며 셀의 내용을 출력하기
    printf("행갯수: %d\n", wks.rowCount());
    printf("열갯수: %d\n", wks.columnCount());

    for (unsigned int row = 1; row <= wks.rowCount(); ++row) {
        for (unsigned int col = 1; col <= wks.columnCount(); ++col) {
            XLCell cell = wks.cell(XLCellReference(row, col));
            XLCellValue cellValue = cell.value();
            wchar_t str[256] = { 0, };

            switch (cellValue.type()) {
            case XLValueType::Empty:
                memcpy(str, L"Empty", 6*sizeof(wchar_t));
                break;
            case XLValueType::Integer:
                swprintf_s(str, 256, L"%d", cellValue.get<int>());
                break;
            case XLValueType::Float:
                swprintf_s(str, 256, L"%f", cellValue.get<double>());
                break;
            case XLValueType::String:
                swprintf_s(str, 256, L"%s", utf8_to_wstring(cellValue.get<std::string>()).c_str());
                break;
            case XLValueType::Boolean:
                memcpy(str, L"BooleanValue", 13 * sizeof(wchar_t));
                break;
            case XLValueType::Error:
                memcpy(str, L"Error", 6 * sizeof(wchar_t));
                break;
            }
            _tprintf(_T("%s "), str);
        }
        printf("\n");
    }

    return 0;
}
