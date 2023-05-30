/*
<���̺귯�� ���� ���(Release x64)>
Git Bash �� ��ġ�Ѵ�.
CMake�� ��ġ�Ѵ�.
Visual Studio�� ��ġ�Ѵ� (Visual Studio 17 2022, Visual Studio 16 2019 ��)
C����̺꿡 git bash�� ������ �������� �����ϰ�
git clone https://github.com/tfussell/xlnt.git
�� �Է��Ͽ� ������Ʈ�� ��ġ�Ѵ�.

cd xlnt/third-party/
rm -rf libstudxml
git clone https://git.codesynthesis.com/libstudxml/libstudxml.git
cd ..
mkdir build
cd build
cmake -G "Visual Studio 17 2022" -A x64 ..
cmake --build . --config Release
cmake --install .

����, 64��Ʈ�� �����ϰ� 32��Ʈ�� �����ϰ��� �ϸ� ���� ���� ���ϰ� CMake ĳ�ø� �����ؾ��Ѵ�.
cd C:/xlnt/build
rm -rf CMakeCache.txt CMakeFiles/
cmake -G "Visual Studio 17 2022" -A Win32 ..
cmake --build . --config Release
cmake --install .

<���� ��� ���丮 ���>
C:\xlnt\include

<���� ���̺귯�� ���丮 ���>
C:\xlnt\build\source\Release

<���� ���̺귯��>
xlnt.lib

<dll PATH ����>
Project -> Properties -> Configuration Properties -> Debugging -> Environment ���ٰ�

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
    wb.load(L"C:\\csv\\�ű� ����.xlsx");
    wchar_t cell_value[256] = { 0, };

    int skip_row = 1;               // ������ ���� ��ŵ�� ���� ����

    auto ws = wb.active_sheet();
    // false�� ������ ���Ե��� �ʴ� �� ���� �ǳʶٱ� ���� �ɼ��Դϴ�.
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