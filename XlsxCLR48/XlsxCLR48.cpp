#include "pch.h"
#include <iostream>
#include <vector>
#include <msclr/marshal_cppstd.h>

using namespace System;
using namespace Microsoft::Office::Interop::Excel;

// '참조 추가' -> COM -> Microsoft Excel 16.0 Object Library (ver 1.9) 추가
// vsproj에서 <EmbedInteropTypes>false</EmbedInteropTypes>   <!-- false로 변경 -->
// clean -> Rebuild
// '참조 추가' -> 어셈블리 -> 프레임워크 -> System.Windows.Forms 추가

using namespace System;
using namespace Microsoft::Office::Interop::Excel;

public ref class ExcelCreator
{
public:
    ExcelCreator() {}
    bool createExcelWithHeaders(String^ filePath, array<String^>^ headers)
    {
        bool func_result = false;
        Application^ excel = gcnew ApplicationClass();
        try
        {
            if (System::IO::File::Exists(filePath))
            {
                Console::WriteLine("이미 존재하는 파일명 입니다.");
                func_result = false;
            }
            else
            {
                excel->Visible = false;
                Workbooks^ workbooks = excel->Workbooks;
                Workbook^ workbook = workbooks->Add(Type::Missing);          // 새로추가
                Worksheet^ worksheet = safe_cast<Worksheet^>(workbook->Sheets[1]);      // 첫번째 시트

                // 헤더 작성
                for (int i = 0; i < headers->Length; i++)
                {
                    worksheet->Cells[1, i + 1] = headers[i];
                }

                workbook->SaveAs(filePath, Type::Missing, Type::Missing, Type::Missing, Type::Missing, Type::Missing, XlSaveAsAccessMode::xlNoChange, Type::Missing, Type::Missing, Type::Missing, Type::Missing, Type::Missing);
                workbook->Close(false, Type::Missing, Type::Missing);
                excel->Quit();
                func_result = true;
            }
        }
        catch (Exception^ e)
        {
            func_result = false;
            Console::WriteLine(e->Message);
        }
        delete excel;

        return func_result;
    }

    bool appendExcelData(String^ filePath, array<array<String^>^>^ data)
    {
        bool func_result = false;
        Application^ excel = gcnew ApplicationClass();
        try
        {
            excel->Visible = false;
            Workbooks^ workbooks = excel->Workbooks;
            Workbook^ workbook = workbooks->Open(filePath,
                Type::Missing, // UpdateLinks
                Type::Missing, // ReadOnly
                Type::Missing, // Format
                Type::Missing, // Password
                Type::Missing, // WriteResPassword
                Type::Missing, // IgnoreReadOnlyRecommended
                Type::Missing, // Origin
                Type::Missing, // Delimiter
                Type::Missing, // Editable
                Type::Missing, // Notify
                Type::Missing, // Converter
                Type::Missing, // AddToMru
                Type::Missing, // Local
                Type::Missing  // CorruptLoad
            );
            Worksheet^ worksheet = safe_cast<Worksheet^>(workbook->Sheets[1]);          // 첫번째 시트

            // 데이터 삽입
            long lastRow = worksheet->Cells->SpecialCells(XlCellType::xlCellTypeLastCell, Type::Missing)->Row;
            for (int i = 0; i < data->Length; i++) {
                for (int j = 0; j < data[i]->Length; j++) {
                    worksheet->Cells[lastRow + i + 1, j + 1] = data[i][j];
                }
            }

            // 변경 사항 저장 및 파일 닫기
            workbook->Save();
            workbook->Close(false, Type::Missing, Type::Missing);
            excel->Quit();
            func_result = true;
        }
        catch (Exception^ e)
        {
            Console::WriteLine(e->Message);
            func_result = false;
        }
        delete excel;

        return func_result;
    }

private:

};

class WrapExcelCreator
{
public:
    bool createExcelWithHeaders(const std::string& filePath, const std::vector<std::string>& headers) {
        ExcelCreator^ excelCreator = gcnew ExcelCreator();

        // std::string을 System::String으로 변환
        String^ clrFilePath = gcnew String(filePath.c_str());

        // std::vector<std::string>을 array<System::String^>으로 변환
        array<String^>^ clrHeaders = gcnew array<String^>(headers.size());
        for (size_t i = 0; i < headers.size(); i++) {
            clrHeaders[i] = gcnew String(headers[i].c_str());
        }

        // 메소드 호출
        return excelCreator->createExcelWithHeaders(clrFilePath, clrHeaders);
    }

    bool appendExcelData(const std::string& filePath, const std::vector<std::vector<std::string>>& data) {
        ExcelCreator^ excelCreator = gcnew ExcelCreator();

        // std::string을 System::String으로 변환
        String^ clrFilePath = gcnew String(filePath.c_str());

        // std::vector<std::vector<std::string>>을 array<array<System::String^>^>으로 변환
        array<array<String^>^>^ clrData = gcnew array<array<String^>^>(data.size());
        for (size_t i = 0; i < data.size(); i++) {
            clrData[i] = gcnew array<String^>(data[i].size());
            for (size_t j = 0; j < data[i].size(); j++) {
                clrData[i][j] = gcnew String(data[i][j].c_str());
            }
        }

        // 메소드 호출
        return excelCreator->appendExcelData(clrFilePath, clrData);
    }
};

int main()
{
    WrapExcelCreator app;
    std::string filepath = "C:\\test\\sample5.xlsx";
    std::vector<std::string> header;
    std::vector<std::vector<std::string>> content;
    int rowCount = 5;

    header.push_back("일번");
    header.push_back("이번");
    header.push_back("삼번");
    header.push_back("사번");

    for (int i = 0; i < rowCount; ++i)
    {
        std::vector<std::string> row;
        row.push_back("Data " + std::to_string(i + 1) + ", Column 1");
        row.push_back("Data " + std::to_string(i + 1) + ", Column 2");
        row.push_back("Data " + std::to_string(i + 1) + ", Column 3");
        row.push_back("Data " + std::to_string(i + 1) + ", Column 4");
        content.push_back(row);
    }

    bool result = false;
    result = app.createExcelWithHeaders(filepath, header);
    if (result == true)
    {
        result = app.appendExcelData(filepath, content);
        if (result == true)
        {
            printf("파일 생성 완료\n");
        }
    }

    return 0;
}