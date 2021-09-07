#include <stdio.h>

void Swap(int* a, int* b) {
	int temp = *a;
	*a = *b;
	*b = temp;
}

void printSwap() {
	int a = 6, b = 8;

	Swap(&a, &b);
	printf("a : %d, b : %d\n", a, b);
}

int main51() {

	int data = 0x12345678;
	(short)data = 0x3344;
	printf("%08x\n", data);

	int a = 0x12345678;
	*(short*)((char*)&a + 1) = 0x7788;
	printf("%08x\n", a);

	printSwap();

	return 0;
}