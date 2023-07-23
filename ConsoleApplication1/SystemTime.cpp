#include <windows.h>
#include <iostream>
#include <sstream>
#include <iomanip>

int main_Gettime() {
    SYSTEMTIME st;
    GetLocalTime(&st); // 현재 로컬 시간을 가져옵니다.

    std::cout << "Year: " << st.wYear << std::endl;
    std::cout << "Month: " << st.wMonth << std::endl;
    std::cout << "Day: " << st.wDay << std::endl;
    std::cout << "Hour: " << st.wHour << std::endl;
    std::cout << "Minute: " << st.wMinute << std::endl;
    std::cout << "Second: " << st.wSecond << std::endl;

    char datetime[20] = { 0, };
    sprintf_s(datetime, 20, "%04d-%02d-%02d %02d:%02d:%02d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
    printf("현재시간은 : %s \n", datetime);

    return 0;
}
