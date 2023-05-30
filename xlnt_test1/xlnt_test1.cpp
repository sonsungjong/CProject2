/*
<라이브러리 빌드 방법(Release x64)>
Git Bash 를 설치한다.
CMake를 설치한다.
Visual Studio를 설치한다 (Visual Studio 17 2022, Visual Studio 16 2019 등)
C드라이브에 git bash를 관리자 권한으로 실행하고
git clone https://github.com/tfussell/xlnt.git
를 입력하여 프로젝트를 설치한다.

cd xlnt/third-party/
rm -rf libstudxml
git clone https://git.codesynthesis.com/libstudxml/libstudxml.git
cd ..
mkdir build
cd build
cmake -G "Visual Studio 17 2022" -A x64 ..
cmake --build . --config Release
cmake --install .

만약, 64비트로 빌드하고 32비트도 빌드하고자 하면 이전 빌드 파일과 CMake 캐시를 제거해야한다.
cd C:/xlnt/build
rm -rf CMakeCache.txt CMakeFiles/
cmake -G "Visual Studio 17 2022" -A Win32 ..
cmake --build . --config Release
cmake --install .

<포함 헤더 디렉토리 경로>
C:\xlnt\include

<포함 라이브러리 디렉토리 경로>
C:\xlnt\build\source\Release

<포함 라이브러리>
xlnt.lib

<dll PATH 지정>
Project -> Properties -> Configuration Properties -> Debugging -> Environment 에다가

PATH=C:\xlnt\build\source\Release;
%PATH%;
*/

#include <xlnt/xlnt.hpp>
#include <iostream>
#include <Windows.h>
#include <tchar.h>

std::wstring utf8_to_wstring(const std::string& utf8str)
{
    int wlen = MultiByteToWideChar(CP_UTF8, 0, utf8str.c_str(), -1, NULL, 0);
    std::wstring wstr(wlen, 0);
    MultiByteToWideChar(CP_UTF8, 0, utf8str.c_str(), -1, &wstr[0], wlen);
    return wstr;
}

int main()
{
    _tsetlocale(0, _T("korean"));

    xlnt::workbook wb;
    wb.load(L"C:\\csv\\신궁 군수.xlsx");
    wchar_t cell_value[256] = { 0, };

    int skip_row = 1;               // 위에서 부터 스킵할 행의 개수

    auto ws = wb.active_sheet();
    // false는 범위에 포함되지 않는 빈 셀을 건너뛰기 위한 옵션입니다.
    for (auto row : ws.rows(false))
    {
        if (skip_row > 0) {
            skip_row--;
            continue;
        }

        for (auto cell : row)
        {
            memset(cell_value, 0, sizeof(cell_value));
            memcpy(cell_value, utf8_to_wstring(cell.to_string()).c_str(), 256);
            wprintf(L"%s ", cell_value);
        }
        printf("\n");
    }

    return 0;
}