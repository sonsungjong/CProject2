#include <stdio.h>
// 포인터의 기계어 수준 설명
int main4회2번째() {
	const short a = 3;			// 2byte
	void* p;		// 4byte		(명령기반 -> 자료형기반 : 실수찾아주려고하다가 이해하는데 좋지않게됨)

	p = &a;
	*(short*)p = 44;
	// p = (short*)((char*)p + 1);		// 주소 1만큼 옮김

	printf("%hd\n", a);

	return 0;
}