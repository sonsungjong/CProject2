// MusicPlayer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <Windows.h>

int main()
{
    while (TRUE) 
    {
        int flag;

        printf("��");
        Beep(260, 200);
        Sleep(100);

        printf("��");
        Beep(290, 200);
        Sleep(100);

        printf("��");
        Beep(330, 200);
        Sleep(100);

        printf("��");
        Beep(340, 200);
        Sleep(100);

        printf("��");
        Beep(380, 200);
        Sleep(100);

        printf("��");
        Beep(430, 200);
        Sleep(100);

        printf("��");
        Beep(490, 200);
        Sleep(100);

        printf("��\n");
        Beep(510, 200);
        Sleep(100);
        
        printf("����Ϸ��� ���ڸ�, �������� 0 : ");
        scanf_s("%d", &flag);

        if (flag == 0) {
            break;
        }
        else {
            continue;
        }
    }
    return 0;
}

