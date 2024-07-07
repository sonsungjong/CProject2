#include "pch.h"
#include <Windows.h>

#include "../CrossCppLib/Func.h"

#ifdef _DEBUG
#pragma comment(lib, "../x64/Debug/CrossCppLib.lib")
#else
#pragma comment(lib, "../x64/Release/CrossCppLib.lib")
#endif

int main() {
    HelloWorld();

    (void)getchar();        // 프로그램 종료 방지
    return 0;
}

