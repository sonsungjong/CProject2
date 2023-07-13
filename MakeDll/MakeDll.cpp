// MakeDll.cpp : Defines the exported functions for the DLL.
//

#include "framework.h"
#include "MakeDll.h"


// This is an example of an exported variable
MAKEDLL_API int nMakeDll=2;

// This is an example of an exported function.
MAKEDLL_API int fnMakeDll(void)
{
    return 3;
}

// This is the constructor of a class that has been exported.
CMakeDll::CMakeDll()
{
    return;
}

int CMakeDll::TenTimeNum(int num)
{
    int result = num * 10;

    return result;
}

extern "C" __declspec(dllexport) int sub(int x1, int y1)
{
    return x1 - y1;
}
