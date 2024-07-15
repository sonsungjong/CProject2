#include <iostream>
#include <Windows.h>

std::string WStringToString(const std::wstring& wstr) {
    // �ʿ��� ���� ũ�⸦ ����մϴ�.
    int bufferSize = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);

    std::string str = "";
    if (bufferSize > 0) {
        // ��ȯ�� ���ڿ��� ������ ���۸� �Ҵ��մϴ�.
        str.resize(bufferSize - 1); // �� ���ڸ� ������ ���� ���ڿ� ����
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
