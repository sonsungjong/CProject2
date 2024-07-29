#pragma once
#include <Windows.h>
#include <stdio.h>

namespace Multi
{
	typedef struct ThreadData {
		int nMsgId;
		char* szMsg;
	} ST_ThreadData;

	DWORD WINAPI ThreadFunc(void* pContext);

	void ThreadTester();
}