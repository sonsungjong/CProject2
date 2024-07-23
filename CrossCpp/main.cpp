#include "pch.h"
#include <Windows.h>

#include "../CrossCppLib/Func.h"
#include "p229_STL.h"
#include "Singleton.h"

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

    //char arrBigSize[10 * 1000 * 1000]{ 0 };              // stack 오버플로우 발생
    static char arrBigSize[10 * 1000 * 1000];               // 정상 동작 (큰 데이터는 전역변수나 정적변수로 활용)
    
    memset(Single()->m_bigSize, 0, sizeof(Single()->m_bigSize));
    strncpy_s(Single()->m_bigSize, sizeof(Single()->m_bigSize), "hello world", 12);
    printf("%s\n", Single()->m_bigSize);

    (void)getchar();        // 프로그램 종료 방지
    return 0;
}

