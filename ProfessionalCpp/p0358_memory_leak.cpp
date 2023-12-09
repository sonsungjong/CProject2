#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

#ifdef _DEBUG
	#ifndef DBG_NEW
		#define  DBG_NEW new (_NORMAL_BLOCK, __FILE__, __LINE__)
		#define  new DBG_NEW
	#endif
#endif		//_DEBUG (MFC�� �޸𸮴��� �˻���)

import "HeaderUnits.h";

// �޸� ���� ����
class Simple0358
{
public:
	Simple0358() {
		m_intPtr = new int;
	}
	~Simple0358() {
		delete m_intPtr;
	}
	void setValue(int value) {
		*m_intPtr = value;
	}
private:
	int* m_intPtr;
};

void doSomething0358(Simple0358*& outSimplePtr)
{
	outSimplePtr = new Simple0358;				// ����... ���� ��ü�� �������� �ʾ���.
}

int main0358()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);				// MFC�� �޸𸮴��� �˻��� (���ø����̼� ����� ���ϸ�(����)�� ǥ�õ�)
	//_CrtSetBreakAlloc(160);				// 160��° �޸� �Ҵ� �� ������ �ߴ��ϰ� ����Ÿ� ����

	Simple0358* simple_ptr = new Simple0358;
	doSomething0358(simple_ptr);
	delete simple_ptr;

	return 0;
}