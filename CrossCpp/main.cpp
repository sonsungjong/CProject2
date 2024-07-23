#include "pch.h"
#include <Windows.h>

#include "../CrossCppLib/Func.h"
#include "p229_STL.h"

#ifdef _DEBUG
#pragma comment(lib, "../x64/Debug/CrossCppLib.lib")
#else
#pragma comment(lib, "../x64/Release/CrossCppLib.lib")
#endif

int main() {
    STL229::stdVector();
    
    HelloLib();
    STL229::withoutNew();

    HelloLib();
    STL229::stdList();
    HelloLib();
    STL229::stdDeque();
    HelloLib();
    STL229::stdStack();
    HelloLib();
    STL229::stdQueue();
    HelloLib();

    STL229::stdMap();
    HelloLib();
    STL229::stdUnorderedMap();
    HelloLib();
    STL229::stdMultimap();
    HelloLib();
    STL229::mapSorting();
    HelloLib();

    STL229::stdSet();
    HelloLib();
    STL229::stdUnorderedSet();
    HelloLib();


    (void)getchar();        // 프로그램 종료 방지
    return 0;
}

