#include <iostream>
#include "BasicExcel.hpp"


void createXls()
{
	YExcel::BasicExcel xls;

	xls.New(1);			// 货 颇老 积己
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
	//createXls();
    loadXls();

}