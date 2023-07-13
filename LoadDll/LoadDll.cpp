#include <iostream>
#include <objbase.h>
#include "MakeDll.h"

#pragma comment(lib, "MakeDll.lib")

typedef int (CALLBACK *MYPROC)(int, int);
typedef int (*SubFunction)(int, int);

typedef struct CCal
{
    int x;
    int y;

    int sum() { return x + y; }
    int sub() { return x - y; }
} Point1;

typedef struct CMulDiv : CCal
{
    int x1;
    int y1;

    double mul() { return x * y; }
    double div() { return x / y; }
} Point2;

class CCC : public CMakeDll
{
public:
    CCC() {
        printf("dll의 클래스를 상속받음\n");
    }
};

int main()
{
    HINSTANCE hDllSub = ::LoadLibrary(TEXT("MakeDll.dll"));

    if (hDllSub == NULL)
    {
        printf("Can\'t load [MakeDll.dll].\n");
        return -1;
    }

    CCC ccc;
    CMakeDll cc;
    int tentime = cc.TenTimeNum(3);
    
    
    printf("%d\n", nMakeDll);
    printf("클래스 내부 10배 메서드 : %d\n", tentime);

    SubFunction lpSub = (SubFunction)GetProcAddress(hDllSub, "sub");

    if (lpSub == NULL) {
        printf("Can\'t find \'sub\' function\n");
        FreeLibrary(hDllSub);
        return -1;
    }

    int dOutput = lpSub(3,2);

    FreeLibrary(hDllSub);

    printf("3-2 = %d\n", dOutput);
}
