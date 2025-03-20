#include <Windows.h>
#include <iostream>
#include <tchar.h>
#include <vector>
#include <map>
#include <filesystem>
#include "BasicExcel.hpp"

// UTF-16�� CP949 ANSI ���ڿ��� ��ȯ�ϴ� �Լ�
std::string convertUTF16toCP949(const std::wstring& utf16)
{
    std::string cp949;
    if (!utf16.empty())
    {
        // CP949�� �ڵ������� 949�Դϴ�.
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

    // ���ڰ� �ƴҶ�
    if (!utf16.empty()) {
        // UTF-8 ��ȯ�� �ʿ��� ���� ũ�� ���
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
        // ���� ����� ���� ������ ������ �����մϴ�. 
        std::filesystem::path filePath(_strFilePath);
        std::filesystem::path parentDir = filePath.parent_path();
        if (!parentDir.empty() && !std::filesystem::exists(parentDir)) {
            std::filesystem::create_directories(parentDir);
        }

        // BasicExcel ��ü ���� ��, ��Ʈ 1���� �� ��ũ���� ����ϴ�.
        YExcel::BasicExcel xls;
        xls.New(1);

        YExcel::BasicExcelWorksheet* sheet = xls.GetWorksheet(size_t(0U));
        if (!sheet) {
            std::cerr << "��ũ��Ʈ�� �� �� �����ϴ�." << std::endl;
            return false;
        }

        // _sheet_data�� �� �� �����͸� ��Ʈ�� ����մϴ�.
        // (���⼭�� ��� �� ��ȣ�� 1���� �����Ѵٰ� �����ϹǷ�, ���� �ε��������� ���� 1�� ���ݴϴ�.)
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

            // ���� ���� (���� ���� �� ���ܰ� �߻��� �� �ֽ��ϴ�.)
            xls.SaveAs(_strFilePath.c_str());
        }
    }
    catch (const std::exception& e) {
        std::cerr << "���� ���� ���� ����: " << e.what() << std::endl;
        return false;
    }

    return true;
}

bool load_xls(std::string _strFilePath, std::map<unsigned int, std::map<unsigned int, std::string>>& _sheet_data)
{

        YExcel::BasicExcel xls;
        if (!xls.Load(_strFilePath.c_str())) {
            std::cerr << "���� �ε� ����: " << _strFilePath << std::endl;
            return false;
        }
        YExcel::BasicExcelWorksheet* sheet = xls.GetWorksheet(size_t(0U));
        if (!sheet) {
            std::cerr << "��ũ��Ʈ�� �ҷ��� �� �����ϴ�." << std::endl;
            return false;
        }

        size_t totalRows = sheet->GetTotalRows();
        size_t totalCols = sheet->GetTotalCols();

        // �� ���� ��ȸ�ϸ鼭 ���� _sheet_data�� �����մϴ�.
        for (size_t i = 0; i < totalRows; ++i) {
            for (size_t j = 0; j < totalCols; ++j) {
                YExcel::BasicExcelCell* cell = sheet->Cell(i, j);
                if (cell) {
                    int enumCell = cell->Type();
                    std::string cellValue;
                    // �� Ÿ�Կ� ���� ���� �����ɴϴ�.
                    if (enumCell == YExcel::BasicExcelCell::STRING) {
                        cellValue = cell->GetString();
                    }
                    else if (enumCell == YExcel::BasicExcelCell::WSTRING) {
                        cellValue = convertUTF16toUTF8(cell->GetWString());
                    }
                    else if (enumCell == YExcel::BasicExcelCell::UNDEFINED) {
                        // �� �� �Ǵ� �� �� ����
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
                    // ���� �� ��� �� ��ȣ�� 1���� ����ϵ��� ����
                    _sheet_data[static_cast<unsigned int>(i + 1)][static_cast<unsigned int>(j + 1)] = cellValue;
                }
            }
        }


    return true;
}

void createXls()
{
	YExcel::BasicExcel xls;

	xls.New(1);			// �� ���� ����
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

    // �� ��Ʈ�� ������ UTF8�� ��´� [��][��] = UTF8����
    std::map<unsigned int, std::map<unsigned int, std::string>> excel_data;             // [1][1] : A1
    excel_data[1][1] = convertUTF16toUTF8(L"A1");            // 1�� A��
    excel_data[2][1] = convertUTF16toUTF8(L"A2");            // 2�� A��
    excel_data[1][2] = convertUTF16toUTF8(L"B1");            // 1�� B��
    excel_data[2][2] = convertUTF16toUTF8(L"B2");            // 2�� B��
    excel_data[4][3] = convertUTF16toUTF8(L"C4");            // 4�� C��
    excel_data[5][2] = convertUTF16toUTF8(L"5�� B��");            // 5�� B��

    char szEXEPath[255]{};
    GetCurrentDirectoryA(255, szEXEPath);
    //std::string strFilePath = std::string(szEXEPath) + "\\output.xls";
    std::string strFilePath = std::string(szEXEPath) + "\\sample.xls";
    //std::string strFilePath = std::string(szEXEPath) + "\\����ź_õ��15A017-00_Ȯ��_202503201049.xls";

    //save_xls(strFilePath, excel_data);
    //Sleep(10);

    std::map<unsigned int, std::map<unsigned int, std::string>> load_data;
    load_xls(strFilePath, load_data);

    for (const auto& row_pair : load_data)
    {
        std::wcout << L"Row " << row_pair.first << L": ";

        for (const auto& col_pair : row_pair.second)
        {
            std::wcout << L"[" << col_pair.first << L": " << convertUTF8toUTF16(col_pair.second) << L"] ";          // UTF8�� ��ȯ�ؼ� ���
        }
        printf("\n");
    }

}