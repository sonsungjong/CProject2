// MusicPlayer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <Windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

int main2()
{
    while (TRUE)
    {
        int flag;
        printf("재생시킬 번호를 누르세요(종료 0): ");
        scanf_s("%d", &flag);

        PlaySound(NULL, 0, 0);
        Sleep(10);

        if (flag == 0) {
            PlaySound(NULL, 0, 0);
            Sleep(10);
            printf("음악 프로그램을 종료합니다.\n");
            break;
        }
        else if (flag == 1) {
            printf("1번 노래를 재생합니다.\n");
            PlaySound(TEXT("D:\\sound1.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
            Sleep(10);
        }
        else if (flag == 2) {
            printf("2번 노래를 재생합니다.\n");
            PlaySound(TEXT("D:\\sound2.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
            Sleep(10);
        }
        else {
            continue;
        }
    }
    return 0;
}

