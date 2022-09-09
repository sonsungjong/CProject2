#include <stdio.h>

// static : 전역변수를 특정 파일에 국한
// static(지역) : 지역변수를 전역변수로 취급

void Test()
{
	static int data = 0;
	printf("%d, ", data++);
}

int main1103()
{
	int i;
	for (i = 0; i < 5; i++) Test();

	return 0;
}