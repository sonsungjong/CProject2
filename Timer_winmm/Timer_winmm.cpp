#include <iostream>
#include <string>

// ����� ���̺귯�� �߰�
#include <Windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

// Ÿ�̸� �ݹ� �Լ� ����
void __stdcall TimerCallback(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
{
    static int num = 0;
    printf("Timer triggered! %d\n", ++num);
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
