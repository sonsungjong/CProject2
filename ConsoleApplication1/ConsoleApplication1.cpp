#include <iostream>

int main1()
{
    char* str;
    str = (char*)malloc(4);

    strcpy_s(str, 4,"123");

    std::cout << str << std::endl;
    free(str);

    return 0;
}