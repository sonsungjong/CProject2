#include <stdio.h>

int main() {

	int data = 0x01020304, i = 0, sum = 0;

	while (i < 4) {
		sum = sum + *((char*)&data + i);
		i++;
	}
	printf("sum=%d\n", sum);

	i = sum = 0;
	while (i < 4) {
		sum = sum + (*((char*)&data + i))++;			// &data : �ּ�, i �� �ε���
		i++;
	}
	printf("sum = %d\n", sum);

	// ��ȣ�� �ʹ� ���Ƽ� �׸�����������(���ȣ)�� ����
	// *(������ + i)  --> ������[i]
	i = sum = 0;
	while (i < 4) {
		sum = sum + ((char*)&data)[i]++;
		i++;
	}
	printf("sum = %d\n", sum);

	char* p;
	p = (char*)&data;
	i = sum = 0;
	while (i < 4) {
		sum = sum + *(p + i);
		i++;
	}
	printf("sum = %d\n", sum);

	i = sum = 0;
	while (i < 4) {
		sum = sum + p[i]++;
		i++;
	}
	printf("sum = %d\n", sum);

	p = (char*)&data;
	char* p_limit = p + 4;
	while (p < p_limit) {
		sum = sum + *p;
		p++;
	}
	printf("sum = %d\n", sum);

	return 0;
}