#include <stdio.h>

void PrintValue(int a_value) {
	printf("Value = %d\n", a_value);
}

int main91() {
	// ������ �����Ϳ� �Լ� ������
	// ����� ������ �ʰ� ������ ���� �� �ִ� ���� (�װ� ���� �Դ´�)
	// �̷��� �߻��� � ���� ǥ���� �� ����.

	// ������ ������ : ������ �ּҸ� ����Ŵ, ����� �̷�
	// �Լ� ������ : �Լ��� �ּҸ� ����Ŵ, ������ �̷�

	// ������ ������
	int data = 0;
	int* p;

	data = 5;
	p = &data;
	*p = 6;

	// int -> int*
	// void Print(int a_value) -> void(*)(int)
	void (*p_func)(int);				// ������ ���� �Ѱ��� ���� �Լ��� �Լ�������
	p_func = &PrintValue;						// &��������

	(*p_func)(5);						// p_func(5);

	return 0;
}