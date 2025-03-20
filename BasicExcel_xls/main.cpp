#include <Windows.h>
#include <iostream>
#include <tchar.h>
#include <vector>
#include <map>
#include <filesystem>
#include "BasicExcel.hpp"

// UTF-16을 CP949 ANSI 문자열로 변환하는 함수
std::string convertUTF16toCP949(const std::wstring& utf16)
{
    std::string cp949;
    if (!utf16.empty())
    {
        // CP949는 코드페이지 949입니다.
        int bufferSize = WideCharToMultiByte(949, 0, utf16.c_str(), -1, NULL, 0, NULL, NULL);
        if (bufferSize > 0)
        {
            cp949.resize(bufferSize - 1);
            WideCharToMultiByte(949, 0, utf16.c_str(), -1, &cp949[0], bufferSize, NULL, NULL);
        }
    }
    return cp949;
}

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

bool save_xls(std::string _strFilePath, const std::map<unsigned int, std::map<unsigned int, std::string>>& _sheet_data)
{
    try {
        // 파일 경로의 상위 폴더가 없으면 생성합니다. 
        std::filesystem::path filePath(_strFilePath);
        std::filesystem::path parentDir = filePath.parent_path();
        if (!parentDir.empty() && !std::filesystem::exists(parentDir)) {
            std::filesystem::create_directories(parentDir);
        }

        // BasicExcel 객체 생성 후, 시트 1개로 새 워크북을 만듭니다.
        YExcel::BasicExcel xls;
        xls.New(1);

        YExcel::BasicExcelWorksheet* sheet = xls.GetWorksheet(size_t(0U));
        if (!sheet) {
            std::cerr << "워크시트를 열 수 없습니다." << std::endl;
            return false;
        }

        // _sheet_data의 각 셀 데이터를 시트에 기록합니다.
        // (여기서는 행과 열 번호가 1부터 시작한다고 가정하므로, 실제 인덱스에서는 각각 1을 빼줍니다.)
        for (const auto& rowPair : _sheet_data) {
            if (rowPair.first == 0) {
                continue;
            }
            for (const auto& colPair : rowPair.second) {
                if (colPair.first == 0) {
                    continue;
                }
                int rowIndex = rowPair.first - 1;
                int colIndex = colPair.first - 1;
                if (rowIndex >= 0 && colIndex >= 0)
                {
                    std::wstring wstrData = convertUTF8toUTF16(colPair.second);
                    sheet->Cell(rowIndex, colIndex)->Set(wstrData.c_str());
                }
            }

            // 파일 저장 (저장 실패 시 예외가 발생할 수 있습니다.)
            xls.SaveAs(_strFilePath.c_str());
        }
    }
    catch (const std::exception& e) {
        std::cerr << "엑셀 파일 저장 오류: " << e.what() << std::endl;
        return false;
    }

    return true;
}

bool load_xls(std::string _strFilePath, std::map<unsigned int, std::map<unsigned int, std::string>>& _sheet_data)
{

        YExcel::BasicExcel xls;
        if (!xls.Load(_strFilePath.c_str())) {
            std::cerr << "파일 로드 실패: " << _strFilePath << std::endl;
            return false;
        }
        YExcel::BasicExcelWorksheet* sheet = xls.GetWorksheet(size_t(0U));
        if (!sheet) {
            std::cerr << "워크시트를 불러올 수 없습니다." << std::endl;
            return false;
        }

        size_t totalRows = sheet->GetTotalRows();
        size_t totalCols = sheet->GetTotalCols();

        // 각 셀을 순회하면서 값을 _sheet_data에 저장합니다.
        for (size_t i = 0; i < totalRows; ++i) {
            for (size_t j = 0; j < totalCols; ++j) {
                YExcel::BasicExcelCell* cell = sheet->Cell(i, j);
                if (cell) {
                    int enumCell = cell->Type();
                    std::string cellValue;
                    // 셀 타입에 따라 값을 가져옵니다.
                    if (enumCell == YExcel::BasicExcelCell::STRING) {
                        cellValue = cell->GetString();
                    }
                    else if (enumCell == YExcel::BasicExcelCell::WSTRING) {
                        cellValue = convertUTF16toUTF8(cell->GetWString());
                    }
                    else if (enumCell == YExcel::BasicExcelCell::UNDEFINED) {
                        // 빈 행 또는 알 수 없음
                        continue;
                    }
                    else if (enumCell == YExcel::BasicExcelCell::DOUBLE) {
                        cellValue = std::to_string(cell->GetDouble());
                    }
                    else if (enumCell == YExcel::BasicExcelCell::INT) {
                        cellValue = std::to_string(cell->GetInteger());
                    }
                    else {
                        continue;
                    }
                    // 저장 시 행과 열 번호는 1부터 사용하도록 보정
                    _sheet_data[static_cast<unsigned int>(i + 1)][static_cast<unsigned int>(j + 1)] = cellValue;
                }
            }
        }


    return true;
}

void createXls()
{
	YExcel::BasicExcel xls;

	xls.New(1);			// 새 파일 생성
	YExcel::BasicExcelWorksheet* sheet = xls.GetWorksheet(size_t(0));

	if (sheet)
	{
		sheet->Cell(0, 0)->Set(123);
		sheet->Cell(0, 1)->Set(456.79);
		sheet->Cell(0, 2)->Set("Hello, Excel");
	}

	xls.SaveAs("output.xls");
}

void loadXls()
{
	YExcel::BasicExcel xls;

    if (xls.Load("output.xls")) {
        YExcel::BasicExcelWorksheet* sheet = xls.GetWorksheet(size_t(0));
        if (sheet) {
            size_t rows = sheet->GetTotalRows();
            size_t cols = sheet->GetTotalCols();
            for (size_t i = 0; i < rows; ++i) {
                for (size_t j = 0; j < cols; ++j) {
                    YExcel::BasicExcelCell* cell = sheet->Cell(i, j);
                    if (cell) {
                        if (cell->Type() == YExcel::BasicExcelCell::INT) {
                            std::cout << cell->GetInteger() << "\t";
                        }
                        else if (cell->Type() == YExcel::BasicExcelCell::DOUBLE) {
                            std::cout << cell->GetDouble() << "\t";
                        }
                        else if (cell->Type() == YExcel::BasicExcelCell::STRING) {
                            std::cout << cell->GetString() << "\t";
                        }
                    }
                }
                std::cout << std::endl;
            }
        }
    }
}

void main()
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
    //std::string strFilePath = std::string(szEXEPath) + "\\output.xls";
    std::string strFilePath = std::string(szEXEPath) + "\\sample.xls";
    //std::string strFilePath = std::string(szEXEPath) + "\\유도탄_천궁15A017-00_확인_202503201049.xls";

    //save_xls(strFilePath, excel_data);
    //Sleep(10);

    std::map<unsigned int, std::map<unsigned int, std::string>> load_data;
    load_xls(strFilePath, load_data);

    for (const auto& row_pair : load_data)
    {
        std::wcout << L"Row " << row_pair.first << L": ";

        for (const auto& col_pair : row_pair.second)
        {
            std::wcout << L"[" << col_pair.first << L": " << convertUTF8toUTF16(col_pair.second) << L"] ";          // UTF8을 변환해서 사용
        }
        printf("\n");
    }

}