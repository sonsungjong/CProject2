#include <stdio.h>

// �Լ��� �޸� ������ ����ǹǷ� �ּ� �Ҵ��
int MyFunc(int num1, int num2)
{
	printf("num1 : %d, num2 : %d\n", num1, num2);
}

int FirstFunc(int a, int b) {}
double SecondFunc(double d) {}
void ThirdFunc(char* c) {}

int main_266()
{
	// �Ű������� int�� 2��
	// int ��ȯ�ϴ� �Լ��� �ּҸ� �����ϴ�
	// �Լ� ������ fptr
	int (*fptr)(int, int);
	fptr = MyFunc;
	fptr(9, 6);

	int (*first)(int, int);
	double (*second)(double);
	void (*third)(char*);

	first = FirstFunc;
	second = SecondFunc;
	third = ThirdFunc;

	return 0;
}