#include <stdio.h>

int main4ȸ() {
	// *������ �ּҰ��� ���� ������ ���
	int a = 1000;		// �������� ����
	int* p = (int*)1000;		// �������� �ƴ� �ּҰ��� ����

	p = &a;
	printf("%d\n", &a);
	printf("%d\n", p);
	printf("%d\n", *p);

	*p = *p - 500;
	printf("%d\n", a);

	return 0;
}