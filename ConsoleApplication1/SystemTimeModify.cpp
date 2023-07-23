#include <Windows.h>
#include <iostream>

int SetSystemTimeInSeoul(int year, int month, int day, int hour, int minute, int second)
{
    SYSTEMTIME st;

    st.wYear = year; // 년도를 변경합니다.
    st.wMonth = month; // 월을 변경합니다.
    st.wDay = day; // 일을 변경합니다.
    st.wHour = (hour + 15) % 24; // 시간을 변경합니다. (한국)
    st.wMinute = minute; // 분을 변경합니다.
    st.wSecond = second; // 초를 변경합니다.

    int result = SetSystemTime(&st);            // 윈도우 시간 변경

    return result;
}

int main() {

    // 윈도우 시간 변경
    if (SetSystemTimeInSeoul(2023, 7, 24, 13, 46, 7)) {
        printf("변경 완료\n");
    }
    else {
        printf("오류 발생\n");
    }

    return 0;
}
