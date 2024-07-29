#include "pch.h"
#include "p331_thread.h"
#include <vector>

// 별도 스레드에서 사용할 함수
DWORD __stdcall Multi::ThreadFunc(void* pContext)
{
	ST_ThreadData& data = *(ST_ThreadData*)pContext;			// 구조체를 포인터변수로 받는다

	if (data.nMsgId == 1)				// 메시지 아이디 식별
	{
		int nMsg = 0;				// 메시지 아이디에 맞는 자료형 준비
		memcpy(&nMsg, data.szMsg, sizeof(int));			// 메모리카피
		printf("스레드가 뭔가 처리 중... 메시지 ID: %d, 메시지: %d\n", data.nMsgId, nMsg);
		for (int i = 0; i < 100000; i++) {
			nMsg++;
		}
		memcpy(data.szMsg, &nMsg, sizeof(int));			// 결과를 다시 메모리카피
		
		printf("완료... 메시지 ID: %d, 처리된 메시지: %d\n", data.nMsgId, nMsg);
	}
	else 
	{
		printf("식별되지 않은 메시지 아이디 : %d\n", data.nMsgId);
	}

	return 0;
}

void Multi::ThreadTester()
{
	ST_ThreadData data = { 0 };
	data.nMsgId = 1;
	int num = 30;
	double dbNum = 3.3;

	static std::vector<char> vecBuffer;				// 메시지를 보관할 벡터
	if (data.nMsgId == 1) {
		vecBuffer.resize(sizeof(int));
		memcpy(vecBuffer.data(), &num, sizeof(int));
	}
	else if (data.nMsgId == 2) 
	{
		vecBuffer.resize(sizeof(double));
		memcpy(vecBuffer.data(), &dbNum, sizeof(double));
	}

	data.szMsg = vecBuffer.data();

	HANDLE hThread = CreateThread(nullptr, 0, ThreadFunc, &data, 0, nullptr);			// 스레드 생성
	DWORD dwRet = WaitForSingleObject(hThread, INFINITE);				// 스레드 종료 대기
	if (WAIT_OBJECT_0 == dwRet) {
		int result = 0;
		memcpy(&result, data.szMsg, sizeof(int));
		printf("작업 진행 -> %d 완료\n", result);
	}
	
	CloseHandle(hThread);				// 스레드 종료
	printf("스레드 종료\n");
}
