// __Console_Tester.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#pragma pack(push, 1)
typedef struct DataFormat {
    long long curTime;    // ms
    int fullSize;         // 8 + 4 + 4 + x + 1
    int id;               // �Ʒü���&��� ����ĺ���
    //std::string data;     // ���̳ʸ� ������ (����ü)
    char padding;         // ETX(3)
}stDataFormat;
#pragma pack(pop)

int main()
{
    printf("%lld\n", sizeof(stDataFormat));
}
