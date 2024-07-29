#include "pch.h"
#include "p331_thread.h"
#include <vector>

// ���� �����忡�� ����� �Լ�
DWORD __stdcall Multi::ThreadFunc(void* pContext)
{
	ST_ThreadData& data = *(ST_ThreadData*)pContext;			// ����ü�� �����ͺ����� �޴´�

	if (data.nMsgId == 1)				// �޽��� ���̵� �ĺ�
	{
		int nMsg = 0;				// �޽��� ���̵� �´� �ڷ��� �غ�
		memcpy(&nMsg, data.szMsg, sizeof(int));			// �޸�ī��
		printf("�����尡 ���� ó�� ��... �޽��� ID: %d, �޽���: %d\n", data.nMsgId, nMsg);
		for (int i = 0; i < 100000; i++) {
			nMsg++;
		}
		memcpy(data.szMsg, &nMsg, sizeof(int));			// ����� �ٽ� �޸�ī��
		
		printf("�Ϸ�... �޽��� ID: %d, ó���� �޽���: %d\n", data.nMsgId, nMsg);
	}
	else 
	{
		printf("�ĺ����� ���� �޽��� ���̵� : %d\n", data.nMsgId);
	}

	return 0;
}

void Multi::ThreadTester()
{
	ST_ThreadData data = { 0 };
	data.nMsgId = 1;
	int num = 30;
	double dbNum = 3.3;

	static std::vector<char> vecBuffer;				// �޽����� ������ ����
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

	HANDLE hThread = CreateThread(nullptr, 0, ThreadFunc, &data, 0, nullptr);			// ������ ����
	DWORD dwRet = WaitForSingleObject(hThread, INFINITE);				// ������ ���� ���
	if (WAIT_OBJECT_0 == dwRet) {
		int result = 0;
		memcpy(&result, data.szMsg, sizeof(int));
		printf("�۾� ���� -> %d �Ϸ�\n", result);
	}
	
	CloseHandle(hThread);				// ������ ����
	printf("������ ����\n");
}
