#include <Windows.h>
#include <iostream>
#include <tchar.h>
#include <conio.h>

#define BUF_SIZE 256

int mainB()
{
    HANDLE hMapFile;
    TCHAR* pBuf;

    hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, _T("MyFileMapping"));

    if (hMapFile == NULL)
    {
        printf("Failed\n");
        return 1;
    }

    pBuf = (TCHAR*)MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, BUF_SIZE);

    if (pBuf == NULL)
    {
        printf("Failed\n");
        CloseHandle(hMapFile);
        return 1;
    }

    MessageBox(NULL, pBuf, _T("Hmmmmm"), MB_OK);
    UnmapViewOfFile(pBuf);
    CloseHandle(hMapFile);

    (void)_getch();
}