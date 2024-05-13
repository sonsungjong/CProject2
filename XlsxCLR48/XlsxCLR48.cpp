#include "pch.h"

using namespace System;
using namespace Microsoft::Office::Interop::Excel;

// '참조 추가' -> COM -> Microsoft Excel 16.0 Object Library (ver 1.9) 추가
// vsproj에서 <EmbedInteropTypes>false</EmbedInteropTypes>   <!-- false로 변경 -->
// clean -> Rebuild

int main(array<System::String ^> ^args)
{
    String^ filePath = "C:\\test\\example4.xlsx";
    try
    {
        if (System::IO::File::Exists(filePath))
        {
            Console::WriteLine("이미 존재하는 파일명 입니다.");
        }
        else
        {
            Application^ excelApp = gcnew ApplicationClass();
            if (excelApp == nullptr)
            {
                Console::WriteLine("Excel could not be started. Check that your office installation and project references are correct.");
                return -1;
            }

            excelApp->Visible = true;

            // 새 워크북 생성
            Workbooks^ workbooks = excelApp->Workbooks;
            Workbook^ workbook = workbooks->Add(Type::Missing);
            Worksheet^ worksheet = safe_cast<Worksheet^>(workbook->Sheets[1]);

            // 셀에 데이터 입력
            worksheet->Cells[1, 1] = "Hello";
            worksheet->Cells[1, 2] = "World";

            // Excel 종료와 정리
            workbook->SaveAs(filePath, Type::Missing, Type::Missing, Type::Missing, Type::Missing, Type::Missing, XlSaveAsAccessMode::xlNoChange, Type::Missing, Type::Missing, Type::Missing, Type::Missing, Type::Missing);
            workbook->Close(false, Type::Missing, Type::Missing);
            excelApp->Quit();
            delete excelApp;
        }
    }
    catch (Exception^ e)
    {
        Console::WriteLine(e->Message);
    }

    return 0;
}
