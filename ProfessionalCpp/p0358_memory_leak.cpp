#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

#ifdef _DEBUG
	#ifndef DBG_NEW
		#define  DBG_NEW new (_NORMAL_BLOCK, __FILE__, __LINE__)
		#define  new DBG_NEW
	#endif
#endif		//_DEBUG (MFC용 메모리누수 검사툴)

import "HeaderUnits.h";

// 메모리 누수 예시
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
	outSimplePtr = new Simple0358;				// 버그... 원본 객체를 삭제하지 않았음.
}

int main0358()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);				// MFC용 메모리누수 검사툴 (애플리케이션 종료시 파일명(라인)이 표시됨)
	//_CrtSetBreakAlloc(160);				// 160번째 메모리 할당 시 실행을 중단하고 디버거를 구동

	Simple0358* simple_ptr = new Simple0358;
	doSomething0358(simple_ptr);
	delete simple_ptr;

	return 0;
}