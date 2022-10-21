#include <stdio.h>

// 코드 영역 (Code) : 코드를 저장
// 데이터 영역 (Data) : 전역변수와 static변수를 저장
// 힙 영역 (Heap) : 동적할당 변수를 저장
// 스택 영역 (Stack) : 지역변수와 매개변수를 저장

int outer = 10;

int main_212()
{
	int inner = 3;
	inner++;
	printf("%d\n", inner);
	inner++;

	return 0;
}