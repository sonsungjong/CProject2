#include <stdio.h>

// # : ��ó���� (�ڵ� ���� ���� �̸� ó���� ��)
#define NUMBER1 3

int sum93(int value1, int value2)
{
	int result = value1 + value2;
	return result;
}

int main093() 
{
	int s;
	s = sum93(NUMBER1, 5);
	printf("���ϱ� �۾��� ����� %d \n", s);

	return 0;
}