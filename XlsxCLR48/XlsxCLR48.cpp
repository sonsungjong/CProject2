#include "pch.h"

using namespace System;
using namespace Microsoft::Office::Interop::Excel;

// '���� �߰�' -> COM -> Microsoft Excel 16.0 Object Library (ver 1.9) �߰�
// vsproj���� <EmbedInteropTypes>false</EmbedInteropTypes>   <!-- false�� ���� -->
// clean -> Rebuild

int main(array<System::String ^> ^args)
{
    Application^ excelApp = gcnew ApplicationClass();
    if (excelApp == nullptr)
    {
        Console::WriteLine("Excel could not be started. Check that your office installation and project references are correct.");
        return -1;
    }

    excelApp->Visible = true;

    // �� ��ũ�� ����
    Workbooks^ workbooks = excelApp->Workbooks;
    Workbook^ workbook = workbooks->Add(Type::Missing);
    Worksheet^ worksheet = safe_cast<Worksheet^>(workbook->Sheets[1]);

    // ���� ������ �Է�
    worksheet->Cells[1, 1] = "Hello";
    worksheet->Cells[1, 2] = "World";
    worksheet->Cells[1, 3] = "!!!";

    // Excel ����� ����
    String^ filePath = "C:\\test\\example3.xlsx";
    workbook->SaveAs(filePath, Type::Missing, Type::Missing, Type::Missing, Type::Missing, Type::Missing, XlSaveAsAccessMode::xlNoChange, Type::Missing, Type::Missing, Type::Missing, Type::Missing, Type::Missing);
    workbook->Close(false, Type::Missing, Type::Missing);
    excelApp->Quit();
    delete excelApp;

    return 0;
}
