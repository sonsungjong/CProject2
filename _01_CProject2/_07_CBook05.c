#include <stdio.h>

void main0705() {
	// ��������
	int x = 5, y = 3;					// �ʱⰪ ����
	int result1 = 0 || 1;					// OR
	int result2 = 3 && -1;					// AND
	int result3 = x == 3 || y == 3;
	int result4 = x == 3 && y == 3;
	int result5 = !x;					// NOT

	printf("result : %d, %d, %d, %d, %d\n", result1, result2, result3, result4, result5);
}