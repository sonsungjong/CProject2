#include <stdio.h>

int Sum(int value1, int value2) {
	int result = value1 + value2;
	return result;
}

void main0501() {
	int s;
	s = Sum(2, 3);
	printf("���ϱ� �۾��� ����� %d \n", s);
}