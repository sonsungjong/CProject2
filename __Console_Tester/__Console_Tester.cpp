#include <iostream>
#include <Windows.h>

std::string WStringToString(const std::wstring& wstr) {
    // 필요한 버퍼 크기를 계산합니다.
    int bufferSize = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);

    std::string str = "";
    if (bufferSize > 0) {
        // 변환된 문자열을 저장할 버퍼를 할당합니다.
        str.resize(bufferSize - 1); // 널 문자를 제외한 실제 문자열 길이
        WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, &str[0], bufferSize, nullptr, nullptr);
    }

    return str;
}

int main()
{
    std::locale::global(std::locale("korean"));

    std::wstring wstr = L"1";
    std::string str = "";

    str = WStringToString(wstr);
    printf("%s\n", str.c_str());

    return 0;
}
