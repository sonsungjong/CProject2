#include "pch.h"

using namespace System;
using namespace System::IO;
using namespace NPOI::XSSF::UserModel;              // XLSX 전용
using namespace NPOI::HSSF::UserModel;              // XLS 전용
using namespace NPOI::SS::UserModel;

void createXlsx()
{
    IWorkbook^ workbook = gcnew XSSFWorkbook();
    ISheet^ sheet = workbook->CreateSheet("시트1");

    // A1 셀에 "안녕하세요" (행 0, 열 0)
    IRow^ row0 = sheet->CreateRow(0);
    ICell^ cellA1 = row0->CreateCell(0);
    cellA1->SetCellValue("안녕하세요");

    // B2 셀에 "반갑습니다" (행 1, 열 1)
    IRow^ row1 = sheet->CreateRow(1);
    ICell^ cellB2 = row1->CreateCell(1);
    cellB2->SetCellValue("반갑습니다");

    // C1 셀에 3 (행 0, 열 2)
    ICell^ cellC1 = row0->CreateCell(2);
    cellC1->SetCellValue("3");

    // C2 셀에 4 (행 1, 열 2)
    ICell^ cellC2 = row1->CreateCell(2);
    cellC2->SetCellValue("4");

    // C3 셀에 수식 =SUM(C1:C2) (행 2, 열 2)
    IRow^ row2 = sheet->CreateRow(2);
    ICell^ cellC3 = row2->CreateCell(2);
    cellC3->SetCellFormula("SUM(C1:C2)");

    // 파일 스트림을 열어 워크북 내용을 파일에 기록
    String^ filePath = "./output/test.xlsx";
    // output 폴더의 경로 추출 후, 폴더가 없으면 생성
    String^ folderPath = Path::GetDirectoryName(filePath);
    if (!Directory::Exists(folderPath))
    {
        Directory::CreateDirectory(folderPath);
    }
    FileStream^ fs = gcnew FileStream(filePath, FileMode::Create, FileAccess::Write);
    workbook->Write(fs, false);                  // true : 파일을 계속 열어둔다, false : 파일을 열어두지 않는다
    fs->Close();
}

void readXlsx()
{
    // Excel 파일 열기
    String^ filePath = "./output/test.xlsx";
    // output 폴더의 경로 추출 후, 폴더가 없으면 생성
    String^ folderPath = Path::GetDirectoryName(filePath);
    if (!Directory::Exists(folderPath))
    {
        Directory::CreateDirectory(folderPath);
    }
    FileStream^ fs = gcnew FileStream(filePath, FileMode::Open, FileAccess::ReadWrite);
    IWorkbook^ workbook = gcnew XSSFWorkbook(fs, false);
    fs->Close();

    // 첫 번째 시트 가져오기
    ISheet^ sheet = workbook->GetSheetAt(0);

    // 첫 번째 행과 첫 번째 셀의 값 읽기
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

// XLS 파일 생성 예시
void createXls()
{
    // XLS 파일용 워크북 생성 (HSSFWorkbook)
    IWorkbook^ workbook = gcnew HSSFWorkbook();
    // 새 시트 생성
    ISheet^ sheet = workbook->CreateSheet("Sheet1");

    // 첫 번째 행 생성 (인덱스 0)
    IRow^ row = sheet->CreateRow(0);
    // 첫 번째 셀 생성 (인덱스 0) 및 값 설정
    ICell^ cell = row->CreateCell(0);
    cell->SetCellValue("Hello, NPOI (XLS)!");

    // 파일 스트림을 열어 워크북 내용을 파일에 기록
    String^ filePath = "./output/test.xls";
    // output 폴더의 경로 추출 후, 폴더가 없으면 생성
    String^ folderPath = Path::GetDirectoryName(filePath);
    if (!Directory::Exists(folderPath))
    {
        Directory::CreateDirectory(folderPath);
    }
    FileStream^ fs = gcnew FileStream(filePath, FileMode::Create, FileAccess::Write);
    // 두 번째 인자(false)는 Write 후 내부적으로 스트림을 닫도록 함
    workbook->Write(fs, false);
    fs->Close();
}

void readXls()
{
    // Excel 파일 열기 (읽기/쓰기 모드로 예시)
    String^ filePath = "./output/test.xls";
    // output 폴더의 경로 추출 후, 폴더가 없으면 생성
    String^ folderPath = Path::GetDirectoryName(filePath);
    if (!Directory::Exists(folderPath))
    {
        Directory::CreateDirectory(folderPath);
    }
    FileStream^ fs = gcnew FileStream(filePath, FileMode::Open, FileAccess::ReadWrite);
    // HSSFWorkbook 생성자 (두 번째 인자: 보통 false 사용 - 매크로 등 특별한 경우가 아니면 false 권장)
    IWorkbook^ workbook = gcnew HSSFWorkbook(fs, false);
    fs->Close();

    // 첫 번째 시트 가져오기
    ISheet^ sheet = workbook->GetSheetAt(0);

    // 첫 번째 행과 첫 번째 셀의 값 읽기
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


    Console::WriteLine("파일 쓰기 완료");
    return 0;
}
