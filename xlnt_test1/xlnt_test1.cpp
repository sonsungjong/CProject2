/*
https://github.com/tfussell/xlnt
Git Bash 를 설치한다.
CMake를 설치한다.
Visual Studio를 설치한다 (Visual Studio 17 2022, Visual Studio 16 2019 등)
C드라이브에 git bash를 관리자 권한으로 실행하고

git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh
./vcpkg integrate install
./vcpkg install xlnt


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
C:\xlnt\build\source\Debug
C:\xlnt\build\source\Release

<포함 라이브러리>
xlntd.lib
xlnt.lib

<dll PATH 지정 또는 환경변수설정 또는 실행파일옆에 위치>
Project -> Properties -> Configuration Properties -> Debugging -> Environment 에다가

PATH=C:\xlnt\build\source\Release;
%PATH%;

*/
#include <iostream>
#include <xlnt/xlnt.hpp>
#include <Windows.h>
#include <tchar.h>
#include <vector>
#include <map>

#ifdef _DEBUG
#pragma comment(lib, "xlnt//xlntd.lib")
#else
#pragma comment(lib, "xlnt//xlnt.lib")
#endif

std::wstring convertUTF8toUTF16(const std::string& utf8)
{
    std::wstring strUTF16 = L"";

    if (!utf8.empty()) {
        int bufferSize = MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, NULL, 0);
        if (bufferSize > 0) {
            strUTF16.assign(static_cast<size_t>(bufferSize) - 1, L'\0');
            MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, &strUTF16[0], bufferSize);
        }
    }

    return strUTF16;
}

std::string convertUTF16toUTF8(const std::wstring& utf16)
{
    std::string strUTF8 = "";

    // 빈문자가 아닐때
    if (!utf16.empty()) {
        // UTF-8 변환에 필요한 버퍼 크기 계산
        int bufferSize = WideCharToMultiByte(CP_UTF8, 0, utf16.c_str(), -1, NULL, 0, NULL, NULL);
        if (bufferSize > 0) {
            strUTF8.assign(static_cast<size_t>(bufferSize) - 1, '\0');
            WideCharToMultiByte(CP_UTF8, 0, utf16.c_str(), -1, &strUTF8[0], bufferSize, NULL, NULL);
        }
    }

    return strUTF8;
}

void save_xlnt()
{
    xlnt::workbook wb;
    xlnt::worksheet ws = wb.active_sheet();                                     // 시트를 연다
    xlnt::alignment align;
    align.vertical(xlnt::vertical_alignment::center);
    //align.horizontal(xlnt::horizontal_alignment::center);

    ws.row_height(8.25);
    
    ws.cell("A9").value(5);                  // 원래 A1 -> A9
    std::wstring wstr = L"한글 문자열";
    std::string utf8KO = convertUTF16toUTF8(wstr);
    ws.cell("B10").value(utf8KO);       // 원래 B2 -> B10
    ws.cell("C11").formula("=SUM(A9, 3)");         // 원래 C3 -> C11
    
	ws.merge_cells("C11:C12");    		                // 셀 C3와 C4를 병합한다.  
	//ws.freeze_panes("B2");                              // 셀 B2 를 기준으로 고정 영역을 만든다

    for (auto row : ws.rows(false)) 
    {
		for (auto cell : row)
		{
			cell.alignment(align);
		}
    }
    wb.save("example.xlsx");
}

const std::map<unsigned int, std::map<unsigned int, std::string>>& load_xlnt()
{
    std::map<unsigned int, std::map<unsigned int, std::string>> sheet_data;             // 행, 열 (0,0에는 파일명, 0,1에는 첫번째 시트명, 0,2에는 두번째 시트명 등) [1][1] 부터 내용 시작

    xlnt::workbook wb;
    try {
        xlnt::path ca = xlnt::path("example.xlsx");
        wb.load(ca);
    }
    catch (const std::exception& e) {
        std::cerr << "엑셀 파일 로드 오류: " << e.what() << std::endl;
        return sheet_data;
    }

    auto ws = wb.active_sheet();                // 시트를 읽는다 (sheet_by_title, sheet_by_index)
    
    for (auto row : ws.rows(false))
    {
        if (row.empty()) {
            continue;               // 비어있으면 건너뛴다
        }

        // 행 번호
        unsigned int row_number = (*row.begin()).reference().row();
        std::map<unsigned int, std::string> row_data;
        
        // 각 셀에 대해
        for (auto cell : row)
        {
            std::string cell_str = cell.to_string();                             // value<int>() 도 가능
            if (!cell_str.empty()) {
                // 열 정보 (A : 1, B : 2, ...)
                unsigned int col_number = cell.reference().column().index;
				row_data[col_number] = cell_str;
            }
            else if (cell.has_formula())
            {
                std::string cell_formula = cell.formula();
                unsigned int col_number = cell.reference().column().index;
                row_data[col_number] = cell_formula;
            }
        }

        if (!row_data.empty()) {
            sheet_data[row_number] = row_data;                // 행에 데이터가 있을 때 저장
        }
    }




    // 저장 데이터 출력 (디버깅용)
    for (const auto& row_pair : sheet_data)
    {
        std::wcout << L"Row " << row_pair.first << L": ";

        for (const auto& col_pair : row_pair.second)
        {
            std::wcout << L"[" << col_pair.first << L": " << convertUTF8toUTF16(col_pair.second) << L"] ";
        }
        printf("\n");
    }

    return sheet_data;
}

int main()
{
    _tsetlocale(0, _T("korean"));

    save_xlnt();
    load_xlnt();

    return 0;
}
