#include <stdio.h>

void main0704() {
	// 관계연산자

	int data1 = 5, data2 = 3;

	int result1 = data1 > 7;
	int result2 = data2 <= data1;
	int result3 = data2 == 7;
	int result4 = data2 != data1;

	printf("result : %d, %d, %d, %d\n", result1, result2, result3, result4);
	// 효율을 위해 거짓은 0, 참은 0외의 값
}