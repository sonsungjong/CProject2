#include "pch.h"
#include <iostream>
#include <vector>
#include <string>
#include <msclr/marshal_cppstd.h>
#include <vcclr.h>

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace Microsoft::Office::Interop::Excel;

// '���� �߰�' -> ������� -> �����ӿ�ũ -> System.Windows.Forms �߰�
// '���� �߰�' -> COM -> Microsoft Excel 16.0 Object Library (ver 1.9) �߰�
// vsproj���� <EmbedInteropTypes>false</EmbedInteropTypes>   <!-- false�� ���� -->
// clean -> Rebuild

// �������� ���� Interop.Microsoft.Office.Interop.Excel.1.9.dll �� ExcelCreator.dll�� �־����

public ref class CoreExcelCreator
{
public:
    CoreExcelCreator()
    {
        m_workbook = nullptr;
        m_excel = nullptr;
    }

    ~CoreExcelCreator()
    {
        if (m_workbook != nullptr)
        {
            m_workbook->Close(false, Type::Missing, Type::Missing);
        }
        if (m_excel != nullptr)
        {
            m_excel->Quit();
            delete m_excel;
        }
    }

    bool createExcelWithHeaders(String^ filePath, array<String^>^ headers)
    {
        bool func_result = false;
        Microsoft::Office::Interop::Excel::Application^ excel = gcnew ApplicationClass();
        try
        {
            if (System::IO::File::Exists(filePath))
            {
                Console::WriteLine("�̹� �����ϴ� ���ϸ� �Դϴ�.");
                func_result = false;
            }
            else
            {
                excel->Visible = false;
                Workbooks^ workbooks = excel->Workbooks;
                Workbook^ workbook = workbooks->Add(Type::Missing);          // �����߰�
                Worksheet^ worksheet = safe_cast<Worksheet^>(workbook->Sheets[1]);      // ù��° ��Ʈ

                // ��� �ۼ�
                for (int i = 0; i < headers->Length; i++)
                {
                    worksheet->Cells[1, i + 1] = headers[i];
                }

                // workbook->SaveAs(filePath, XlFileFormat::xlExcel8, Type::Missing, Type::Missing, Type::Missing, Type::Missing, XlSaveAsAccessMode::xlNoChange, Type::Missing, Type::Missing, Type::Missing, Type::Missing, Type::Missing);       // xls
                workbook->SaveAs(filePath, Type::Missing, Type::Missing, Type::Missing, Type::Missing, Type::Missing, XlSaveAsAccessMode::xlNoChange, Type::Missing, Type::Missing, Type::Missing, Type::Missing, Type::Missing);                   // xlsx
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
        Microsoft::Office::Interop::Excel::Application^ excel = gcnew ApplicationClass();
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
            Worksheet^ worksheet = safe_cast<Worksheet^>(workbook->Sheets[1]);          // ù��° ��Ʈ

            // ������ ����
            long lastRow = worksheet->Cells->SpecialCells(XlCellType::xlCellTypeLastCell, Type::Missing)->Row;
            for (int i = 0; i < data->Length; i++) {
                for (int j = 0; j < data[i]->Length; j++) {
                    worksheet->Cells[lastRow + i + 1, j + 1] = data[i][j];
                }
            }

            // ���� ���� ���� �� ���� �ݱ�
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

    String^ readOnceCell(String^ filePath, String^ sheetName, String^ columnName, int rowNumber)
    {
        Microsoft::Office::Interop::Excel::Application^ excel = gcnew ApplicationClass();
        try
        {
            excel->Visible = false;
            Workbooks^ workbooks = excel->Workbooks;
            Workbook^ workbook = workbooks->Open(filePath,
                Type::Missing, // UpdateLinks
                true, // ReadOnly
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
            Worksheet^ worksheet = safe_cast<Worksheet^>(workbook->Sheets[sheetName]);
            Range^ cell = worksheet->Range[columnName + rowNumber.ToString(), Type::Missing];

            String^ value = nullptr;
            if (cell->Value2 != nullptr) {
                value = cell->Value2->ToString();           // ���� ���� ���ڿ��� ��ȯ
            }
            workbook->Close(false, Type::Missing, Type::Missing);
            excel->Quit();
            delete excel;
            return value;
        }
        catch (Exception^ e)
        {
            Console::WriteLine(e->Message);
            if (excel != nullptr)
            {
                excel->Quit();
                delete excel;
            }
            return nullptr;
        }
    }

    bool readFile(String^ filePath, String^ sheetName)
    {
        try
        {
            if (System::IO::File::Exists(filePath))
            {
                m_excel = gcnew ApplicationClass();
                m_excel->Visible = false;
                Workbooks^ workbooks = m_excel->Workbooks;
                m_workbook = workbooks->Open(filePath,
                    Type::Missing, // UpdateLinks
                    true, // ReadOnly
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
                m_worksheet = safe_cast<Worksheet^>(m_workbook->Sheets[sheetName]);
            }
            else
            {
                Console::WriteLine("������ �������� �ʽ��ϴ�.");
                return false;
            }
        }
        catch (Exception^ e)
        {
            Console::WriteLine(e->Message->ToString());
            return false;
        }

        return true;
    }

    String^ readCell(String^ columnName, int rowNumber)
    {
        Range^ cell = m_worksheet->Range[columnName + rowNumber.ToString(), Type::Missing];

        String^ value = nullptr;
        if (cell != nullptr && cell->Value2 != nullptr) 
        {
            value = cell->Value2->ToString();           // ���� ���� ���ڿ��� ��ȯ
        }
        return value;
    }

    void readEnd()
    {
        if (m_workbook != nullptr)
        {
            m_workbook->Close(false, Type::Missing, Type::Missing);
            m_workbook = nullptr;
        }
        if (m_excel != nullptr)
        {
            m_excel->Quit();
            delete m_excel;
            m_excel = nullptr;
        }
    }

private:
    Microsoft::Office::Interop::Excel::Application^ m_excel;
    Workbook^ m_workbook;
    Worksheet^ m_worksheet;
};

class ExcelCreator
{
public:
    bool createExcelWithHeaders(const std::string& filePath, const std::vector<std::string>& headers)
    {
        CoreExcelCreator^ excelCreator = gcnew CoreExcelCreator();

        // std::string�� System::String���� ��ȯ
        String^ clrFilePath = gcnew String(filePath.c_str());

        // std::vector<std::string>�� array<System::String^>���� ��ȯ
        array<String^>^ clrHeaders = gcnew array<String^>(headers.size());
        for (size_t i = 0; i < headers.size(); i++) {
            clrHeaders[i] = gcnew String(headers[i].c_str());
        }

        // �޼ҵ� ȣ��
        return excelCreator->createExcelWithHeaders(clrFilePath, clrHeaders);
    }

    bool appendExcelData(const std::string& filePath, const std::vector<std::vector<std::string>>& data) 
    {
        CoreExcelCreator^ excelCreator = gcnew CoreExcelCreator();

        // std::string�� System::String���� ��ȯ
        String^ clrFilePath = gcnew String(filePath.c_str());

        // std::vector<std::vector<std::string>>�� array<array<System::String^>^>���� ��ȯ
        array<array<String^>^>^ clrData = gcnew array<array<String^>^>(data.size());
        for (size_t i = 0; i < data.size(); i++) {
            clrData[i] = gcnew array<String^>(data[i].size());
            for (size_t j = 0; j < data[i].size(); j++) {
                clrData[i][j] = gcnew String(data[i][j].c_str());
            }
        }

        // �޼ҵ� ȣ��
        return excelCreator->appendExcelData(clrFilePath, clrData);
    }

    std::string readOnceCell(const std::string& filePath, const std::string& sheetName, const std::string& columnName, int rowNumber)
    {
        CoreExcelCreator^ excelCreator = gcnew CoreExcelCreator();
        String^ clrFilePath = gcnew String(filePath.c_str());
        String^ clrSheetName = gcnew String(sheetName.c_str());
        String^ clrColumnName = gcnew String(columnName.c_str());

        String^ result = excelCreator->readOnceCell(clrFilePath, clrSheetName, clrColumnName, rowNumber);
        std::string value;
        if (result != nullptr)
        {
            value = msclr::interop::marshal_as<std::string>(result);
        }
        else
        {
            value = std::string();
        }

        return value;
    }

    bool readFile(const std::string& filePath, const std::string& sheetName)
    {
        // void* �� ��ü�Ҵ�
        CoreExcelCreator^ newCreator = gcnew CoreExcelCreator();          // ����� ���
        GCHandle gch = GCHandle::Alloc(newCreator);
        m_excelCreator = GCHandle::ToIntPtr(gch).ToPointer();

        String^ clrFilePath = gcnew String(filePath.c_str());
        String^ clrSheetName = gcnew String(sheetName.c_str());

        // void*�� ����ȯ
        CoreExcelCreator^ creator = safe_cast<CoreExcelCreator^>(GCHandle::FromIntPtr(IntPtr(m_excelCreator)).Target);
        bool result = creator->readFile(clrFilePath, clrSheetName);
        return result;
    }

    std::string readCell(const std::string& columnName, int rowNumber)
    {
        String^ clrColumnName = gcnew String(columnName.c_str());

        GCHandle gch = GCHandle::FromIntPtr(IntPtr(m_excelCreator));
        CoreExcelCreator^ creator = safe_cast<CoreExcelCreator^>(gch.Target);
        String^ clrValue = creator->readCell(clrColumnName, rowNumber);

        std::string value = std::string();
        if (clrValue != nullptr)
        {
            value = msclr::interop::marshal_as<std::string>(clrValue);
        }

        return value;
    }

    void readEnd()
    {
        GCHandle gch = GCHandle::FromIntPtr(IntPtr(m_excelCreator));
        CoreExcelCreator^ creator = safe_cast<CoreExcelCreator^>(gch.Target);
        creator->readEnd();
        m_excelCreator = nullptr;      // ������ �÷��Ͱ� ������ �� �ְ� nullptr��
    }

private:
    void* m_excelCreator;
};

int main()
{
    ExcelCreator app;
    
    std::string filepath = "C:\\test\\sample.xlsx";
    
    std::vector<std::string> header;
    std::vector<std::vector<std::string>> content;
    int rowCount = 5;

    header.push_back("�Ϲ�");
    header.push_back("�̹�");
    header.push_back("���");
    header.push_back("���");

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
            printf("���� ���� �Ϸ�\n");
        }
    }

    std::string value = "";
    value = app.readOnceCell("C:\\test\\�����׸�.xls", "81082502", "D", 8);
    printf("%s\n", value.c_str());

    // readFile, readCell, readEnd �� ��Ʈ�� ����� �� (readCell ������ ����ϱ�)
    app.readFile("C:\\test\\sample.xlsx", "Sheet1");
    printf("%s\n", app.readCell("C", 3).c_str());
    printf("%s\n", app.readCell("D", 3).c_str());
    printf("%s\n", app.readCell("A", 3).c_str());
    app.readEnd();

    app.readFile("C:\\test\\�����׸�.xls", "81082502");
    printf("%s\n", app.readCell("D", 8).c_str());
    printf("%s\n", app.readCell("C", 9).c_str());
    app.readEnd();

    app.readFile("C:\\test\\�����׸�.xls", "81082502");
    printf("%s\n", app.readCell("F", 439).c_str());
    printf("%s\n", app.readCell("D", 8).c_str());
    app.readEnd();


    printf("���α׷� ����\n");

    return 0;
}