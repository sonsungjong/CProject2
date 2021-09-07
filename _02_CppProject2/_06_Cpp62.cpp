#include <iostream>

void _swapPointer(int* ap_a, int* ap_b) {
	int temp;
	temp = *ap_a;
	*ap_a = *ap_b;
	*ap_b = temp;
}
void _swapRef(int& ar_a, int& ar_b) {
	int temp;
	temp = ar_a;
	ar_a = ar_b;
	ar_b = temp;
}

int main62() {
	int a = 3, b = 6;
	int data = 0;
	int& r = data;
	int* p = &data;

	r = 8;		// data = 8;
	*p = 9;		// data = 9; 

	// reference : 포인터를 편하게 쓰라고 만든 문법
	_swapPointer(&a, &b);
	_swapRef(a, b);

	return 0;
}