#include <iostream>
#include <string>

// 헤더와 라이브러리 추가
#include <Windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

// 타이머 콜백 함수 정의
void __stdcall TimerCallback(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
{
    static int num = 0;
    printf("Timer triggered! %d\n", ++num);
}

int main()
{
    // 타이머 해상도 설정
    TIMECAPS tc;
    if (timeGetDevCaps(&tc, sizeof(TIMECAPS)) != TIMERR_NOERROR)
    {
        printf("Failed to get timer capabilities.\n");
        return 1;
    }
    timeBeginPeriod(tc.wPeriodMin);

    // 타이머 이벤트 설정 (주기성 32ms, 정밀도 1ms, 콜백함수, 매개변수, 주기사용여부)
    UINT timerInterval = 32;
    UINT timerID = timeSetEvent(timerInterval, 1, TimerCallback, 0, TIME_PERIODIC);
    if (timerID == 0)
    {
        printf("Failed to set timer.\n");
        timeEndPeriod(tc.wPeriodMin);
        return 1;
    }

    while (true)
    {
        std::string input = "";
        std::getline(std::cin, input);
        if (input == "1")
        {
            printf("정상속도로 원복 (32ms마다)\n");
            timerInterval = 32;
        }
        else if (input == "2")
        {
            printf("2배속 (16ms마다)\n");
            timerInterval = 16;
        }
        else if (input == "4")
        {
            printf("4배속 (8ms마다)\n");
            timerInterval = 8;
        }
        else if (input == "8")
        {
            printf("8배속 (4ms마다)\n");
            timerInterval = 4;
        }
        else if (input == "1000")
        {
            printf("1초 (1s마다)\n");
            timerInterval = 1000;
        }
        else if (input == "500")
        {
            printf("0.5초 (500ms마다)\n");
            timerInterval = 500;
        }
        else if (input == "" || input == "0")
        {
            break;
        }
        else
        {
            continue;
        }

        // 기존 타이머 중지
        timeKillEvent(timerID);

        // 새로운 주기로 타이머 설정
        timerID = timeSetEvent(timerInterval, 1, TimerCallback, 0, TIME_PERIODIC);
        if (timerID == 0)
        {
            printf("Failed to set timer.\n");
            timeEndPeriod(tc.wPeriodMin);
            return 1;
        }
    }

    // 타이머를 중지합니다.
    timeKillEvent(timerID);
    timeEndPeriod(tc.wPeriodMin);

    return 0;
}
