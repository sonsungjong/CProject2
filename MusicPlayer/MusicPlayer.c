// MusicPlayer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <Windows.h>

int main()
{
    while (TRUE) 
    {
        int flag;

        printf("도");
        Beep(260, 200);
        Sleep(100);

        printf("레");
        Beep(290, 200);
        Sleep(100);

        printf("미");
        Beep(330, 200);
        Sleep(100);

        printf("파");
        Beep(340, 200);
        Sleep(100);

        printf("솔");
        Beep(380, 200);
        Sleep(100);

        printf("라");
        Beep(430, 200);
        Sleep(100);

        printf("시");
        Beep(490, 200);
        Sleep(100);

        printf("도\n");
        Beep(510, 200);
        Sleep(100);
        
        printf("계속하려면 숫자를, 끝내려면 0 : ");
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

