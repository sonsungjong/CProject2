#include <iostream>
#include <string>
#include <chrono>

// 헤더와 라이브러리 추가 (윈도우 멀티미디어 타이머)
#include <Windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

// 출력용
char* convertMsToTime(long long milliseconds) {
    const long long msPerSecond = 1000;
    const long long msPerMinute = msPerSecond * 60;
    const long long msPerHour = msPerMinute * 60;
    const long long msPerDay = msPerHour * 24;

    long long days = milliseconds / msPerDay;
    milliseconds -= days * msPerDay;
    long long hours = milliseconds / msPerHour;
    milliseconds -= hours * msPerHour;
    long long minutes = milliseconds / msPerMinute;
    milliseconds -= minutes * msPerMinute;
    long long seconds = milliseconds / msPerSecond;
    milliseconds -= seconds * msPerSecond;

    char* str = (char*)malloc(256 * sizeof(char));          // 할당
    if (str == NULL) {
        exit(EXIT_FAILURE);
    }
    sprintf_s(str, 256, "%lld일 %lld시간 %lld분 %lld초 %lld밀리초\n", days, hours, minutes, seconds, milliseconds);
    return str;
}

// 타이머 콜백 함수 정의
void __stdcall TimerCallback(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
{
    static int num = 0;
    static long long time = 0;

    time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    char* timeString = convertMsToTime(time);
    printf("Time: %s, Count: %d\n", timeString, ++num);
    free(timeString);           // 해제
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
