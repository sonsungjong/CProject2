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
        // ASCII 변환에 필요한 버퍼 크기 계산
        int bufferSize = WideCharToMultiByte(CP_ACP, 0, utf16.c_str(), -1, NULL, 0, NULL, NULL);
        if (bufferSize > 0) {
            strASCII.assign(static_cast<size_t>(bufferSize) - 1, '\0');  // 널 문자를 제외한 크기로 할당
            WideCharToMultiByte(CP_ACP, 0, utf16.c_str(), -1, &strASCII[0], bufferSize, NULL, NULL);
        }
    }

    return strASCII;
}

// UTF-16을 CP949 ANSI 문자열로 변환하는 함수
std::string convertUTF16toCP949(const std::wstring& utf16)
{
    std::string cp949;
    if (!utf16.empty())
    {
        // CP949는 코드페이지 949입니다.
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

    // 빈문자가 아닐때
    if (!utf16.empty()) {
        // UTF-8 변환에 필요한 버퍼 크기 계산
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
        std::cerr << "Python 실행 실패\n";
        return;
    }

    char buffer[4096]{};  // 한 줄씩 받을 버퍼
    std::vector<std::vector<std::string>> data;  // 2D 배열 저장
    std::string line;

    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        line = buffer;
        std::vector<std::string> row;
        std::stringstream ss(line);
        std::string cell;

        // CSV 형식으로 파싱 (쉼표 `,` 기준)
        while (std::getline(ss, cell, ',')) {
            // 앞뒤 공백 제거
            cell.erase(0, cell.find_first_not_of(" \t\r\n"));
            cell.erase(cell.find_last_not_of(" \t\r\n") + 1);
            row.push_back(cell);
        }
        data.push_back(row);
    }
    _pclose(pipe);

    // 배열 출력 확인
    std::cout << "엑셀 데이터 (배열 형태):\n";
    for (const auto& row : data) {
        for (const auto& cell : row) {
            std::cout << "[" << cell << "] ";
        }
        std::cout << std::endl;
    }
}

void testMap(const std::wstring& filePath, std::map<unsigned int, std::map<unsigned int, std::string>>& excel_data)
{
    // Python 스크립트 실행
    std::wstring command = L"python ";
    //std::wstring command = L"wpython ";               // 콘솔창 안보이게하려면
    command += L"C:\\CBMPlusServer\\exe\\xlrd_python.py \"";
    command += filePath;
    command += L"\"";

    FILE* pipe = _wpopen(command.c_str(), L"r");
    if (pipe) {
        char buffer[1024];  // 한 줄씩 받을 버퍼 
        while (fgets(buffer, sizeof(buffer) / sizeof(buffer[0]), pipe) != nullptr) {
            std::istringstream iss(buffer);
            unsigned int row, col;
            std::string value;

            if (!(iss >> row >> col)) {
                continue;  // 잘못된 형식 무시
            }

            std::getline(iss, value); // 값 읽기
            if (!value.empty() && value[0] == L' ') {
                value.erase(0, 1);  // 앞쪽 공백 제거
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