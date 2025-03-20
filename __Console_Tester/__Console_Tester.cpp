#include <Windows.h>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <filesystem>

std::string convertUTF16toASCII(const std::wstring& utf16)
{
    std::string strASCII = "";

    if (!utf16.empty()) {
        // ASCII ��ȯ�� �ʿ��� ���� ũ�� ���
        int bufferSize = WideCharToMultiByte(CP_ACP, 0, utf16.c_str(), -1, NULL, 0, NULL, NULL);
        if (bufferSize > 0) {
            strASCII.assign(static_cast<size_t>(bufferSize) - 1, '\0');  // �� ���ڸ� ������ ũ��� �Ҵ�
            WideCharToMultiByte(CP_ACP, 0, utf16.c_str(), -1, &strASCII[0], bufferSize, NULL, NULL);
        }
    }

    return strASCII;
}

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

void testVector()
{
    FILE* pipe = _popen("python C:\\Users\\Administrator\\source\\repos\\CProject2\\__Console_Tester\\tester.py", "r");
    if (!pipe) {
        std::cerr << "Python ���� ����\n";
        return;
    }

    char buffer[4096]{};  // �� �پ� ���� ����
    std::vector<std::vector<std::string>> data;  // 2D �迭 ����
    std::string line;

    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        line = buffer;
        std::vector<std::string> row;
        std::stringstream ss(line);
        std::string cell;

        // CSV �������� �Ľ� (��ǥ `,` ����)
        while (std::getline(ss, cell, ',')) {
            // �յ� ���� ����
            cell.erase(0, cell.find_first_not_of(" \t\r\n"));
            cell.erase(cell.find_last_not_of(" \t\r\n") + 1);
            row.push_back(cell);
        }
        data.push_back(row);
    }
    _pclose(pipe);

    // �迭 ��� Ȯ��
    std::cout << "���� ������ (�迭 ����):\n";
    for (const auto& row : data) {
        for (const auto& cell : row) {
            std::cout << "[" << cell << "] ";
        }
        std::cout << std::endl;
    }
}

void testMap(const std::wstring& filePath, std::map<unsigned int, std::map<unsigned int, std::string>>& excel_data)
{
    // Python ��ũ��Ʈ ����
    std::wstring command = L"python ";
    //std::wstring command = L"wpython ";               // �ܼ�â �Ⱥ��̰��Ϸ���
    command += L"C:\\CBMPlusServer\\exe\\xlrd_python.py \"";
    command += filePath;
    command += L"\"";

    FILE* pipe = _wpopen(command.c_str(), L"r");
    if (pipe) {
        char buffer[1024];  // �� �پ� ���� ���� 
        while (fgets(buffer, sizeof(buffer) / sizeof(buffer[0]), pipe) != nullptr) {
            std::istringstream iss(buffer);
            unsigned int row, col;
            std::string value;

            if (!(iss >> row >> col)) {
                continue;  // �߸��� ���� ����
            }

            std::getline(iss, value); // �� �б�
            if (!value.empty() && value[0] == L' ') {
                value.erase(0, 1);  // ���� ���� ����
            }

            excel_data[row][col] = value;
        }

        _pclose(pipe);
    }
}

void main()
{
    std::filesystem::current_path().wstring();
    std::wstring filePath = std::filesystem::current_path().wstring() + L"\\sample.xls";
    //testVector();
    std::map<unsigned int, std::map<unsigned int, std::string>> test;
    testMap(filePath, test);

    for (const auto& row : test) {
        for (const auto& col : row.second) {
            std::wstring wstr = convertUTF8toUTF16(col.second);
            std::string second = convertUTF16toASCII(wstr);
            std::cout << "[" << row.first << "][" << col.first << "] = " << second << "  ";
        }
        printf("\n");
    }
}