#ifndef RING_MEMORY_H_
#define RING_MEMORY_H_

#include <new>

/*
메모리 순환을 위한 클래스
반복되는 할당을 피하기 위해 사용
충분히 큰 사이즈로 Create하여 생성하고
Alloc으로 가져다 쓰게 한다
Destroy로 링메모리를 해제한다
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