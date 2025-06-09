/*
<���̺귯�� ���� ���(Release x64)>
Git Bash �� ��ġ�Ѵ�.
CMake�� ��ġ�Ѵ�.
Visual Studio�� ��ġ�Ѵ� (Visual Studio 17 2022, Visual Studio 16 2019 ��)
C����̺꿡 git bash�� ������ �������� �����ϰ�
git clone https://github.com/troldal/OpenXLSX.git
�� �Է��Ͽ� ������Ʈ�� ��ġ�Ѵ�.

cd OpenXLSX
mkdir build
cd build
cmake -G "Visual Studio 17 2022" -A x64 ..
cmake --build . --config Release
cmake --install .

����, 64��Ʈ�� �����ϰ� 32��Ʈ�� �����ϰ��� �ϸ� ���� ���� ���ϰ� CMake ĳ�ø� �����ؾ��Ѵ�.
cd C:/OpenXLSX/build
rm -rf CMakeCache.txt CMakeFiles/
cmake -G "Visual Studio 17 2022" -A Win32 ..
cmake --build . --target OpenXLSX --config Release
cmake --install .

<�߰����� 1>
C:\OpenXLSX\build\OpenXLSX ��ο� �ִ�
OpenXLSX-Exports.hpp ������ �����Ͽ�
C:\OpenXLSX\OpenXLSX\headers ��ο� �ٿ��־� ����մϴ�.

<�߰����� 2>
������Ʈ �Ӽ����� C++ ��� ǥ���� ISO C++17 Standard (/std:c++17) �� ��������� �մϴ�.


<���� ��� ���丮 ���>
C:\OpenXLSX

<���� ���̺귯�� ���丮 ���>
C:\OpenXLSX\build\output\Release

<���� ���̺귯��>
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
    std::wstring filepath = L"C:\\csv\\�ű� ����.xlsx";
    std::wstring sheetname = L"������ ����";

    // ���� ���� ����
    XLDocument doc;
    doc.open(wstring_to_utf8(filepath));

    // �ش� ��Ʈ�̸� ��������
    XLWorksheet wks = doc.workbook().worksheet(wstring_to_utf8(sheetname));

    // ��ũ��Ʈ�� ��� ��� ���� ��ȸ�ϸ� ���� ������ ����ϱ�
    printf("�హ��: %d\n", wks.rowCount());
    printf("������: %d\n", wks.columnCount());

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
