#include "pch.h"

using namespace System;
using namespace System::IO;
using namespace NPOI::XSSF::UserModel;              // XLSX ����
using namespace NPOI::HSSF::UserModel;              // XLS ����
using namespace NPOI::SS::UserModel;

void createXlsx()
{
    IWorkbook^ workbook = gcnew XSSFWorkbook();
    ISheet^ sheet = workbook->CreateSheet("��Ʈ1");

    // A1 ���� "�ȳ��ϼ���" (�� 0, �� 0)
    IRow^ row0 = sheet->CreateRow(0);
    ICell^ cellA1 = row0->CreateCell(0);
    cellA1->SetCellValue("�ȳ��ϼ���");

    // B2 ���� "�ݰ����ϴ�" (�� 1, �� 1)
    IRow^ row1 = sheet->CreateRow(1);
    ICell^ cellB2 = row1->CreateCell(1);
    cellB2->SetCellValue("�ݰ����ϴ�");

    // C1 ���� 3 (�� 0, �� 2)
    ICell^ cellC1 = row0->CreateCell(2);
    cellC1->SetCellValue("3");

    // C2 ���� 4 (�� 1, �� 2)
    ICell^ cellC2 = row1->CreateCell(2);
    cellC2->SetCellValue("4");

    // C3 ���� ���� =SUM(C1:C2) (�� 2, �� 2)
    IRow^ row2 = sheet->CreateRow(2);
    ICell^ cellC3 = row2->CreateCell(2);
    cellC3->SetCellFormula("SUM(C1:C2)");

    // ���� ��Ʈ���� ���� ��ũ�� ������ ���Ͽ� ���
    String^ filePath = "./output/test.xlsx";
    // output ������ ��� ���� ��, ������ ������ ����
    String^ folderPath = Path::GetDirectoryName(filePath);
    if (!Directory::Exists(folderPath))
    {
        Directory::CreateDirectory(folderPath);
    }
    FileStream^ fs = gcnew FileStream(filePath, FileMode::Create, FileAccess::Write);
    workbook->Write(fs, false);                  // true : ������ ��� ����д�, false : ������ ������� �ʴ´�
    fs->Close();
}

void readXlsx()
{
    // Excel ���� ����
    String^ filePath = "./output/test.xlsx";
    // output ������ ��� ���� ��, ������ ������ ����
    String^ folderPath = Path::GetDirectoryName(filePath);
    if (!Directory::Exists(folderPath))
    {
        Directory::CreateDirectory(folderPath);
    }
    FileStream^ fs = gcnew FileStream(filePath, FileMode::Open, FileAccess::ReadWrite);
    IWorkbook^ workbook = gcnew XSSFWorkbook(fs, false);
    fs->Close();

    // ù ��° ��Ʈ ��������
    ISheet^ sheet = workbook->GetSheetAt(0);

    // ù ��° ��� ù ��° ���� �� �б�
    IRow^ row = sheet->GetRow(0);
    if (row != nullptr)
    {
        ICell^ cell = row->GetCell(0);
        if (cell != nullptr)
        {
            String^ cellValue = cell->ToString();
            Console::WriteLine("Cell Value: {0}", cellValue);
        }
    }
}

// XLS ���� ���� ����
void createXls()
{
    // XLS ���Ͽ� ��ũ�� ���� (HSSFWorkbook)
    IWorkbook^ workbook = gcnew HSSFWorkbook();
    // �� ��Ʈ ����
    ISheet^ sheet = workbook->CreateSheet("Sheet1");

    // ù ��° �� ���� (�ε��� 0)
    IRow^ row = sheet->CreateRow(0);
    // ù ��° �� ���� (�ε��� 0) �� �� ����
    ICell^ cell = row->CreateCell(0);
    cell->SetCellValue("Hello, NPOI (XLS)!");

    // ���� ��Ʈ���� ���� ��ũ�� ������ ���Ͽ� ���
    String^ filePath = "./output/test.xls";
    // output ������ ��� ���� ��, ������ ������ ����
    String^ folderPath = Path::GetDirectoryName(filePath);
    if (!Directory::Exists(folderPath))
    {
        Directory::CreateDirectory(folderPath);
    }
    FileStream^ fs = gcnew FileStream(filePath, FileMode::Create, FileAccess::Write);
    // �� ��° ����(false)�� Write �� ���������� ��Ʈ���� �ݵ��� ��
    workbook->Write(fs, false);
    fs->Close();
}

void readXls()
{
    // Excel ���� ���� (�б�/���� ���� ����)
    String^ filePath = "./output/test.xls";
    // output ������ ��� ���� ��, ������ ������ ����
    String^ folderPath = Path::GetDirectoryName(filePath);
    if (!Directory::Exists(folderPath))
    {
        Directory::CreateDirectory(folderPath);
    }
    FileStream^ fs = gcnew FileStream(filePath, FileMode::Open, FileAccess::ReadWrite);
    // HSSFWorkbook ������ (�� ��° ����: ���� false ��� - ��ũ�� �� Ư���� ��찡 �ƴϸ� false ����)
    IWorkbook^ workbook = gcnew HSSFWorkbook(fs, false);
    fs->Close();

    // ù ��° ��Ʈ ��������
    ISheet^ sheet = workbook->GetSheetAt(0);

    // ù ��° ��� ù ��° ���� �� �б�
    IRow^ row = sheet->GetRow(0);
    if (row != nullptr)
    {
        ICell^ cell = row->GetCell(0);
        if (cell != nullptr)
        {
            String^ cellValue = cell->ToString();
            Console::WriteLine("Cell Value: {0}", cellValue);
        }
    }
}

int main(array<System::String ^> ^args)
{
    createXlsx();


    Console::WriteLine("���� ���� �Ϸ�");
    return 0;
}
