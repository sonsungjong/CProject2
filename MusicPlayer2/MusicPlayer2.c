#include <stdio.h>
#include <Windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

int main()
{
    while (TRUE)
    {
        int flag;
        printf("�����ų ��ȣ�� ��������(���� 0): ");
        scanf_s("%d", &flag);

        PlaySound(NULL, 0, 0);
        Sleep(10);

        if (flag == 0) {
            PlaySound(NULL, 0, 0);
            Sleep(10);
            printf("���� ���α׷��� �����մϴ�.\n");
            Sleep(500);
            break;
        }
        else if (flag == 1) {
            printf("1�� �뷡�� ����մϴ�.\n");
            PlaySound(TEXT("C:\\sound1.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
            Sleep(10);
        }
        else if (flag == 2) {
            printf("2�� �뷡�� ����մϴ�.\n");
            PlaySound(TEXT("C:\\sound2.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
            Sleep(10);
        }
        else {
            continue;
        }
    }
    return 0;
}

