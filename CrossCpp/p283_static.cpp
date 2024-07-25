#include "pch.h"
#include "p283_static.h"

int g_nGlobalVar = 0;					// cpp에서 전역변수 정의 및 초기화
int CStatic::ms_num = 0;			// cpp에서 정적멤버변수 정의 및 초기화

void memoryRegion()
{
	static int s_nStaticVar = 0;
	int nLocalVar = 0;
	int* pHeapVar = new int;

	printf("GlobalVar address: 0x%08X\n", &g_nGlobalVar);
	printf("StaticlVar address: 0x%08X\n", &s_nStaticVar);
	printf("LocalVar  address: 0x%08X\n", &nLocalVar);
	printf("HeapVar  address: 0x%08X\n", pHeapVar);
}

void staticGlobalFunction()
{
	printf("This is Static Function in Global\n");
}

void CStatic::memberFunction()
{
	printf("This is Member Function in Class\n");
}

void CStatic::staticMemberFunction()
{
	static int s_num = 0;
	printf("This is Static Member Function in Class, %d %d\n", ++s_num, ++ms_num);
}
