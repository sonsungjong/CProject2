#include <stdio.h>

// �ڵ� ���� (Code) : �ڵ带 ����
// ������ ���� (Data) : ���������� static������ ����
// �� ���� (Heap) : �����Ҵ� ������ ����
// ���� ���� (Stack) : ���������� �Ű������� ����

int outer = 10;

int main_212()
{
	int inner = 3;
	inner++;
	printf("%d\n", inner);
	inner++;

	return 0;
}