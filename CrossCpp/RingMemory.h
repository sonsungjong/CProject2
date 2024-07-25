#ifndef RING_MEMORY_H_
#define RING_MEMORY_H_

#include <new>

/*
�޸� ��ȯ�� ���� Ŭ����
�ݺ��Ǵ� �Ҵ��� ���ϱ� ���� ���
����� ū ������� Create�Ͽ� �����ϰ�
Alloc���� ������ ���� �Ѵ�
Destroy�� ���޸𸮸� �����Ѵ�
*/
class CRingMemory
{
	unsigned char* m_pMemory;
	int m_nMemorySize;
	int m_nLastAllocPos;

public:
	CRingMemory();

	~CRingMemory();

	bool Create(int nMaxSize);

	void Destroy();

	unsigned char* Alloc(int nSize);
};

#endif		RING_MEMORY_H_