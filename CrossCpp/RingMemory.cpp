#include "pch.h"
#include "RingMemory.h"

CRingMemory::CRingMemory()
	:m_pMemory(nullptr)
	, m_nMemorySize(0)
	, m_nLastAllocPos(0)
{}

CRingMemory::~CRingMemory()
{
}

bool CRingMemory::Create(int nMaxSize)
{
	m_pMemory = new(std::nothrow) unsigned char[nMaxSize];
	if (nullptr == m_pMemory) return false;

	m_nMemorySize = 0;
	m_nLastAllocPos = 0;
	return true;
}

void CRingMemory::Destroy()
{
	if (m_pMemory) delete[] m_pMemory;
	m_pMemory = nullptr;
}

unsigned char* CRingMemory::Alloc(int nSize)
{
	if (m_nMemorySize < (m_nLastAllocPos + nSize))
	{
		m_nLastAllocPos = 0;
	}

	unsigned char* pNewMemory = m_pMemory + m_nLastAllocPos;
	m_nLastAllocPos += nSize;
	return pNewMemory;
}