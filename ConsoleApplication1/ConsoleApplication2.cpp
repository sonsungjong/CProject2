#include <cstdio>
#include <Windows.h>

int main2()
{
    // $err,hr 을 Name에 입력
    HANDLE hFile = CreateFile(TEXT("C:\\Jeff"), 0, 0, NULL, OPEN_EXISTING, 0, NULL);
    GetLastError();

    return 0;
}
