#include <stdio.h>

int main4회() {
	// *붙으면 주소값을 갖는 변수로 취급
	int a = 1000;		// 정수값을 갖음
	int* p = (int*)1000;		// 정수값이 아닌 주소값을 갖음

	p = &a;
	printf("%d\n", &a);
	printf("%d\n", p);
	printf("%d\n", *p);

	*p = *p - 500;
	printf("%d\n", a);

	return 0;
}