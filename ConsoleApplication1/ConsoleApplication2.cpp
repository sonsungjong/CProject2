#include <cstdio>
#include <Windows.h>

int main()
{
    // $err,hr �� Name�� �Է�
    HANDLE hFile = CreateFile(TEXT("C:\\Jeff"), 0, 0, NULL, OPEN_EXISTING, 0, NULL);
    GetLastError();

    return 0;
}

/*
    ���̺귯�� ����
    
    
*/