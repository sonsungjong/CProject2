#ifndef EXCEL_CREATOR_H_
#define EXCEL_CREATOR_H_

// .net framework 4.8
namespace InteropExcel
{
	class __declspec(dllexport) ExcelCreator
	{
	public:
		bool createExcelWithHeaders(const std::string& filePath, const std::vector<std::string>& headers);
		bool appendExcelData(const std::string& filePath, const std::vector<std::vector<std::string>>& data);
		bool readFile(const std::string& filePath, const std::string& sheetName);
		std::string readCell(const std::string& columnName, int rowNumber);
		void readEnd();
	private:
		void* m_excelCreator;
	};
}

#endif			EXCEL_CREATOR_H_