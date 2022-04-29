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
		sum = sum + (*((char*)&data + i))++;			// &data : 주소, i 는 인덱스
		i++;
	}
	printf("sum = %d\n", sum);

	// 괄호가 너무 많아서 항목지정연산자(대괄호)를 도입
	// *(변수명 + i)  --> 변수명[i]
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