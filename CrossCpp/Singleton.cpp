#include "pch.h"
#include "Singleton.h"

CSingleton::CSingleton()
{
	memset(m_bigSize, 0, sizeof(m_bigSize) / sizeof(m_bigSize[0]));
}


CSingleton::~CSingleton()
{
}
