#include <stdio.h>

// static : ���������� Ư�� ���Ͽ� ����
// static(����) : ���������� ���������� ���

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