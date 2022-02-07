#include <stdio.h>
#include <malloc.h>

int main71() {
	int data;
	int** p;
	//int temp;
	//temp = (int)&data;					// 4byte면 주소를 담을 수 있다.
	//p = (int**)&temp;

	int* p_temp;
	p_temp = &data;
	p = &p_temp;

	**p = 6;				// data = 6

	return 0;
}