#include <cstdio>
#include <Windows.h>

int main2()
{
    // $err,hr �� Name�� �Է�
    HANDLE hFile = CreateFile(TEXT("C:\\Jeff"), 0, 0, NULL, OPEN_EXISTING, 0, NULL);
    GetLastError();

    return 0;
}
