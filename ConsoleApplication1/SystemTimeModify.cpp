#include <Windows.h>
#include <iostream>

/* 
윈도우 시간변경 예제

관리자 권한으로 실행해야하며,
시간변경 시 성공 시 그 다음 리빌드는 안하므로
수동으로 리빌드를 해줘야함
*/

// UTC시간을 통해 한국시간으로 변환하여 변경한다.
int SetSystemUTCTimeInSeoul(int year, int month, int day, int hour, int minute, int second)
{
    SYSTEMTIME systemtime;
    GetSystemTime(&systemtime);

    systemtime.wYear = year; // 년도를 변경합니다.
    systemtime.wMonth = month; // 월을 변경합니다.
    systemtime.wDay = day; // 일을 변경합니다.
    systemtime.wHour = (hour + 15) % 24; // 시간을 변경합니다. (한국)
    systemtime.wMinute = minute; // 분을 변경합니다.
    systemtime.wSecond = second; // 초를 변경합니다.

    int result = SetSystemTime(&systemtime);            // 윈도우 시간 변경

    return result;
}

// 윈도우 설정지역을 기준으로 시간을 변경현다.
int SetSystemLocalTime(int year, int month, int day, int hour, int minute, int second)
{
    SYSTEMTIME systemtime;
    GetLocalTime(&systemtime);              // 현재 로컬시간을 얻어온다 (초기화용)

    systemtime.wYear = year;
    systemtime.wMonth = month;
    systemtime.wDay = day;
    systemtime.wHour = hour;
    systemtime.wMinute = minute;
    systemtime.wSecond = second;

    int result = SetLocalTime(&systemtime);

    return result;
}

int main() 
{
    int year = 2024;
    printf("년도를 입력하세요>>");
    scanf_s("%d", &year);

    // 윈도우 시간 변경 (관리자 권한 필요, 코드 변경 시 리빌드 필요)
    //int result = SetSystemUTCTimeInSeoul(year, 7, 23, 13, 46, 7);
    int result = SetSystemLocalTime(year, 1, 29, 17, 43, 20);

    if (result == 0)
    {
        printf("오류 발생 %d\n", result);
    }
    else 
    {
        printf("변경 완료 %d\n", result);
    }

    return 0;
}
