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
#include <filesystem>

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

//void save_xlnt_example()
//{
//    xlnt::workbook wb;
//    xlnt::worksheet ws = wb.active_sheet();                                     // 시트를 연다
//    xlnt::alignment align;
//    align.vertical(xlnt::vertical_alignment::center);
//    //align.horizontal(xlnt::horizontal_alignment::center);
//
//    ws.row_height(8.25);
//    
//    ws.cell("A9").value(5);                  // 원래 A1 -> A9
//    std::wstring wstr = L"한글 문자열";
//    std::string utf8KO = convertUTF16toUTF8(wstr);
//    ws.cell("B10").value(utf8KO);       // 원래 B2 -> B10
//    ws.cell("C11").formula("=SUM(A9, 3)");         // 원래 C3 -> C11
//    
//	ws.merge_cells("C11:C12");    		                // 셀 C3와 C4를 병합한다.  
//	//ws.freeze_panes("B2");                              // 셀 B2 를 기준으로 고정 영역을 만든다
//
//    for (auto row : ws.rows(false)) 
//    {
//		for (auto cell : row)
//		{
//			cell.alignment(align);
//		}
//    }
//    wb.save("example.xlsx");
//}

bool save_xlnt(std::string _strFilePath, const std::map<unsigned int, std::map<unsigned int, std::string>>& _sheet_data)
{
    bool bResult = false;

    // 전달받은 파일 경로의 상위 폴더가 없으면 생성
    std::filesystem::path filePath(_strFilePath);
    std::filesystem::path parentDir = filePath.parent_path();
    if (!parentDir.empty() && !std::filesystem::exists(parentDir))
    {
        try {
            std::filesystem::create_directories(parentDir);
        }
        catch (const std::exception& e) {
            std::cerr << "폴더 생성 오류: " << e.what() << std::endl;
        }
    }

    xlnt::workbook wb;
    xlnt::worksheet ws = wb.active_sheet();                                     // 시트를 연다
    xlnt::alignment align;
    align.vertical(xlnt::vertical_alignment::center);
    //align.horizontal(xlnt::horizontal_alignment::center);

    for (const auto& row_pair : _sheet_data)
    {
        if (row_pair.first == 0) {
            continue;			   // 0행은 파일명, 시트명 등이므로 건너뛴다
        }

        for (const auto& col_pair : row_pair.second)
        {
            if (col_pair.first == 0) {
                continue;           // 0번 열은 없음 ( 1: A, 2: B, ...)
            }

            xlnt::cell_reference ref(col_pair.first, row_pair.first);
            ws.cell(ref).value(col_pair.second);
        }
    }

    for (auto row : ws.rows(false))
    {
        for (auto cell : row)
        {
            cell.alignment(align);                  // 수직 가운데 정렬
        }
    }

    try {
        wb.save(_strFilePath);
        bResult = true;
    }
    catch (const std::exception& e) {
        std::cerr << "Excel 파일 저장 오류: " << e.what() << std::endl;
        bResult = false;
    }
    

    return bResult;
}

/*
    행, 열 [1][1] 부터 내용 시작
*/
bool load_xlnt(const std::string& _strFilePath, std::map<unsigned int, std::map<unsigned int, std::string>>& _sheet_data)
{
    bool bResult = false;

    // 파일이 존재하는지 체크
    if (!std::filesystem::exists(_strFilePath))
    {
        std::cerr << "파일이 존재하지 않습니다: " << _strFilePath << std::endl;
        return false;
    }

    xlnt::workbook wb;
    try {
        wb.load(_strFilePath);
        bResult = true;
    }
    catch (const std::exception& e) {
        std::cerr << "엑셀 파일 로드 오류: " << e.what() << std::endl;
        bResult = false;
    }

    if(bResult == true)
    {
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
                _sheet_data[row_number] = row_data;                // 행에 데이터가 있을 때 저장
            }
        }
    }
    
    return bResult;
}

int main()
{
    _tsetlocale(0, _T("korean"));

    // 한 시트의 내용을 UTF8로 담는다 [행][열] = UTF8내용
    std::map<unsigned int, std::map<unsigned int, std::string>> excel_data;             // [1][1] : A1
    excel_data[1][1] = convertUTF16toUTF8(L"A1");            // 1행 A열
    excel_data[2][1] = convertUTF16toUTF8(L"A2");            // 2행 A열
    excel_data[1][2] = convertUTF16toUTF8(L"B1");            // 1행 B열
    excel_data[2][2] = convertUTF16toUTF8(L"B2");            // 2행 B열
    excel_data[4][3] = convertUTF16toUTF8(L"C4");            // 4행 C열
    excel_data[5][2] = convertUTF16toUTF8(L"5행 B열");            // 5행 B열
    
    char szEXEPath[255]{};
    GetCurrentDirectoryA(255, szEXEPath);
    std::string strFilePath = std::string(szEXEPath) + "\\output\\sample.xlsx";

    save_xlnt(strFilePath, excel_data);
    Sleep(10);

    std::map<unsigned int, std::map<unsigned int, std::string>> load_data;
    load_xlnt(strFilePath, load_data);

    for (const auto& row_pair : load_data)
    {
        std::wcout << L"Row " << row_pair.first << L": ";

        for (const auto& col_pair : row_pair.second)
        {
            std::wcout << L"[" << col_pair.first << L": " << convertUTF8toUTF16(col_pair.second) << L"] ";          // UTF8을 변환해서 사용
        }
        printf("\n");
    }

    return 0;
}
