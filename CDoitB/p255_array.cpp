#include <stdio.h>


int main255()
{	
	char data[5] = { 1,2,3,4,5 };

	int result = 0, i;

	for (i = 0; i < 5; i++) {
		result = result + data[i];
	}

	printf("data �迭�� �� ����� ���� %d�Դϴ�\n", result);

	return 0;
}