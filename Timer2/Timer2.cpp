#include <iostream>
#include <string>
#include <chrono>

// ����� ���̺귯�� �߰� (������ ��Ƽ�̵�� Ÿ�̸�)
#include <Windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

// ��¿�
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

    char* str = (char*)malloc(256 * sizeof(char));          // �Ҵ�
    if (str == NULL) {
        exit(EXIT_FAILURE);
    }
    sprintf_s(str, 256, "%lld�� %lld�ð� %lld�� %lld�� %lld�и���\n", days, hours, minutes, seconds, milliseconds);
    return str;
}

// Ÿ�̸� �ݹ� �Լ� ����
void __stdcall TimerCallback(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
{
    static int num = 0;
    static long long time = 0;

    time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    char* timeString = convertMsToTime(time);
    printf("Time: %s, Count: %d\n", timeString, ++num);
    free(timeString);           // ����
}

int main()
{
    // Ÿ�̸� �ػ� ����
    TIMECAPS tc;
    if (timeGetDevCaps(&tc, sizeof(TIMECAPS)) != TIMERR_NOERROR)
    {
        printf("Failed to get timer capabilities.\n");
        return 1;
    }
    timeBeginPeriod(tc.wPeriodMin);

    // Ÿ�̸� �̺�Ʈ ���� (�ֱ⼺ 32ms, ���е� 1ms, �ݹ��Լ�, �Ű�����, �ֱ��뿩��)
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
            printf("����ӵ��� ���� (32ms����)\n");
            timerInterval = 32;
        }
        else if (input == "2")
        {
            printf("2��� (16ms����)\n");
            timerInterval = 16;
        }
        else if (input == "4")
        {
            printf("4��� (8ms����)\n");
            timerInterval = 8;
        }
        else if (input == "8")
        {
            printf("8��� (4ms����)\n");
            timerInterval = 4;
        }
        else if (input == "1000")
        {
            printf("1�� (1s����)\n");
            timerInterval = 1000;
        }
        else if (input == "500")
        {
            printf("0.5�� (500ms����)\n");
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

        // ���� Ÿ�̸� ����
        timeKillEvent(timerID);

        // ���ο� �ֱ�� Ÿ�̸� ����
        timerID = timeSetEvent(timerInterval, 1, TimerCallback, 0, TIME_PERIODIC);
        if (timerID == 0)
        {
            printf("Failed to set timer.\n");
            timeEndPeriod(tc.wPeriodMin);
            return 1;
        }
    }

    // Ÿ�̸Ӹ� �����մϴ�.
    timeKillEvent(timerID);
    timeEndPeriod(tc.wPeriodMin);

    return 0;
}
