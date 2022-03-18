#include <Windows.h>
#include <iostream>
#include <tchar.h>
#include <conio.h>

#define BUF_SIZE 256

int mainA()
{
    HANDLE hMapFile;
    TCHAR* pBuf;

    hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, BUF_SIZE, _T("MyFileMapping"));
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

    CopyMemory((void*)pBuf, "HIII", 4);

    (void)_getch();

    UnmapViewOfFile(pBuf);

    CloseHandle(hMapFile);

    (void)_getch();
}