#include "pch.h"
#include <Windows.h>

#include "../CrossCppLib/Func.h"
#include "p229_STL.h"
#include "Singleton.h"
#include "RingMemory.h"
#include "p283_static.h"
#include "p331_thread.h"

#ifdef _DEBUG
#pragma comment(lib, "../x64/Debug/CrossCppLib.lib")
#else
#pragma comment(lib, "../x64/Release/CrossCppLib.lib")
#endif

typedef struct
{
    int nMsgId;
    char szMsg[256];
} ST_Struct;

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

    HelloLib();
    CRingMemory ring;
    ring.Create(1 * 1000 * 1000);
    for (int i = 0; i < 20; ++i)
    {
        auto* pMemoryToken = ring.Alloc(128 * 1000);
        
        memset(pMemoryToken, 0, 128 * 1000);
        memcpy(pMemoryToken, "Hello World~", 13);
        printf("%s\t", pMemoryToken);
    }

    // 구조체 할당 예제 추가 (링메모리)
    printf("\n");
    int nNumber = 1;
    // 20번
    for (int i = 0; i < 20; ++i) {
        // ST_Struct을 5개 배열로 생성하려 할 때
        unsigned char* pMemoryToken = ring.Alloc(sizeof(ST_Struct) * 5);
        ST_Struct* p_st = reinterpret_cast<ST_Struct*>(pMemoryToken);

        for (int j = 0; j < 5; j++) {
            p_st[j].nMsgId = j + 1;
            std::string strDump = std::to_string(nNumber) + "번째 데이터";
            nNumber++;
            strncpy_s(p_st[j].szMsg, sizeof(p_st[j].szMsg), strDump.c_str(), strDump.size());
        }

        for (int j = 0; j < 5; j++) {
            printf("id: %d, msg: %s\n", p_st[j].nMsgId, p_st[j].szMsg);
        }
        printf("\n===================\n");
    }

    ring.Destroy();

    HelloLib();
    memoryRegion();
    HelloLib();

    CStatic::staticMemberFunction();
    CStatic obj;
    obj.memberFunction();
    obj.staticMemberFunction();
    HelloLib();

    Multi::ThreadTester();
    HelloLib();

    (void)getchar();        // 프로그램 종료 방지
    return 0;
}

